//
// Created by lucas on 07/02/2026.
//
#pragma once

#include <memory>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Renderer.h"

class Application;

// Owns:
// - The loop
// - Timing
// - Window Lifetime
class Engine {

public:
    static constexpr int window_width = {640};
    static constexpr int window_height = {480};

public:
    Engine(Application* app);

    ~Engine();

    void Run();

private:
    GLFWwindow* window = nullptr;
    Application* application = nullptr; //Engine doesn't own Application, thats why its a raw pointer, we just borrow it
    std::unique_ptr<Renderer> renderer = nullptr;

    float lastFrame;
};

