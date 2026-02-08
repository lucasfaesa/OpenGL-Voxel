//
// Created by lucas on 07/02/2026.
//
#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
class Application;
class Renderer;

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

    void Run();

private:
    GLFWwindow* window = nullptr;
    Application* application = nullptr;
    Renderer* renderer = nullptr;

    float lastFrame;
};

