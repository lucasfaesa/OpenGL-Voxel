#include "Engine.h"
#include "Renderer.h"
#include "Shader.h"

static const char* skyboxVertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec3 TexCoords;

    uniform mat4 projection;
    uniform mat4 view;

    void main()
    {
        TexCoords = aPos;
        mat4 staticView = mat4(mat3(view));
        vec4 pos = projection * staticView * vec4(aPos, 1.0);
        gl_Position = pos.xyww;
    }
)";

static const char* skyboxFragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 TexCoords;

    void main()
    {
        vec3 dir = normalize(TexCoords);

        // 1. Atmospheric Gradient
        float t = clamp(dir.y, 0.0, 1.0);
        // A power curve keeps the bright horizon thin and expands the deep blue zenith
        float blendFactor = pow(t, 0.4);

        vec3 horizonColor = vec3(0.7f, 0.85f, 1.0f); // Bright, hazy horizon
        vec3 zenithColor  = vec3(0.15f, 0.35f, 0.75f); // Deep rich blue
        vec3 skyColor = mix(horizonColor, zenithColor, blendFactor);

        // 2. The Sun
        // Static direction for the sun (x, y, z)
        vec3 sunDir = normalize(vec3(0.5, 0.4, -0.8));

        // How closely the camera is looking at the sun (1.0 = direct hit, 0.0 = 90 degrees away)
        float sunAlignment = max(dot(dir, sunDir), 0.0);

        // High exponent creates a sharp, tiny disc
        float sunDisc = pow(sunAlignment, 2000.0) * 1.5;

        // Low exponent creates a wide, soft glow around the disc
        float sunGlow = pow(sunAlignment, 8.0) * 0.3;

        vec3 sunColor = vec3(1.0, 0.95, 0.8) * (sunDisc + sunGlow);

        // Add the sun light to the base sky color
        FragColor = vec4(skyColor + sunColor, 1.0);
    }
)";

Renderer::Renderer() : grass("../assets/grass.jpg")
{
    shader = std::make_unique<Shader>();
    skyShader = std::make_unique<Shader>(skyboxVertexSrc, skyboxFragmentSrc);

    glEnable(GL_DEPTH_TEST);
    SetupSkybox();
}

Renderer::~Renderer()
{
    if (skyboxVAO) glDeleteVertexArrays(1, &skyboxVAO);
    if (skyboxVBO) glDeleteBuffers(1, &skyboxVBO);
}

void Renderer::SetupSkybox()
{
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(World& world, const Camera& camera) {

    // 1. Draw World
    shader->Bind();
    grass.Bind(0);
    shader->SetInt("u_Texture", 0);
    shader->SetMat4("model", glm::mat4(1.0f));

    float aspectRatio = static_cast<float>(Engine::window_width) / Engine::window_height;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = camera.GetProjectionMatrix(aspectRatio);

    shader->SetMat4("view", view);
    shader->SetMat4("projection", proj);

    frustum_.Update(proj * view);
    world.Render(frustum_);

    // 2. Draw Skybox
    glDepthFunc(GL_LEQUAL);
    skyShader->Bind();

    skyShader->SetMat4("view", view);
    skyShader->SetMat4("projection", proj);

    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void Renderer::EndFrame(GLFWwindow *window) {
    glfwSwapBuffers(window);
}