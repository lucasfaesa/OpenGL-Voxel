#include "Engine.h"
#include "Renderer.h"
#include "Application.h"
#include <iostream>

Engine::Engine(Application *app) : application(app), lastFrame(0.0f)
{
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
    }

    window = glfwCreateWindow(window_width, window_height, "Voxel World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
    }

    glViewport(0, 0, window_width, window_height);

    // CRITICAL: Initialize the application's OpenGL resources AFTER gladLoadGLLoader
    application->Init();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer = std::make_unique<Renderer>();
}

void Engine::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        application->ProcessInput(window, deltaTime);
        application->Update(deltaTime);

        renderer->BeginFrame();
        renderer->Draw(*application);
        renderer->EndFrame(window);
    }

    glfwTerminate();
}