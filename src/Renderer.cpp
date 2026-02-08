#include "Engine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Application.h"
#include "Mesh.h"

Renderer::Renderer()
{
    shader = new Shader();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const Application &app) {
    shader->Bind();

    shader->SetMat4("model", app.GetModelMatrix());
    shader->SetMat4("view", app.GetCamera().GetViewMatrix());
    shader->SetMat4("projection", app.GetCamera().GetProjectionMatrix(static_cast<float>(Engine::window_width)/Engine::window_height));

    app.GetMesh()->Draw();
}

void Renderer::EndFrame(GLFWwindow *window) {
    glfwSwapBuffers(window);
}
