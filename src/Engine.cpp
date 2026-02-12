#include "Engine.h"
#include "Renderer.h"
#include "Application.h"

#include "GLDebug.h"
#include "Logger.h"

Engine::Engine(Application *app) : application(app), lastFrame(0.0f)
{
    if (!glfwInit()) {
        LOG_ERROR("Failed to init GLFW");
    }

    window = glfwCreateWindow(window_width, window_height, "Voxel World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to init GLAD");
    }
    glEnable(GL_CULL_FACE);
    //activating openGL debug callbacks
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugOpenGLErrorCallback, nullptr);

    glViewport(0, 0, window_width, window_height);

    // CRITICAL: Initialize the application's OpenGL resources AFTER gladLoadGLLoader
    application->Init();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer = std::make_unique<Renderer>();
}

Engine::~Engine()
{
    renderer.reset(); // Deletes renderer (and shaders) while context is alive
    application->Cleanup(); //Deletes the mesh

    glfwDestroyWindow(window);
    glfwTerminate();
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
    }}
