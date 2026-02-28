#include "Engine.h"
#include "Renderer.h"
#include "Shader.h"

Renderer::Renderer() : grass("../assets/grass.jpg")
{
    shader = std::make_unique<Shader>();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(World& world, const Camera& camera) {

    shader->Bind();
    grass.Bind(0);
    shader->SetInt("u_Texture", 0);
    // We use an identity matrix for 'model' because the chunk
    // positions are already baked into the vertices
    shader->SetMat4("model", glm::mat4(1.0f));
    shader->SetMat4("view", camera.GetViewMatrix());
    shader->SetMat4("projection", camera.GetProjectionMatrix(static_cast<float>(Engine::window_width)/Engine::window_height));

    float aspectRatio = static_cast<float>(Engine::window_width)/ Engine::window_height;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 proj = camera.GetProjectionMatrix(aspectRatio);

    frustum_.Update(proj * view);

    world.Render(frustum_);
}

void Renderer::EndFrame(GLFWwindow *window) {
    glfwSwapBuffers(window);
}
