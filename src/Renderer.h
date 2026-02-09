//
// Created by lucas on 07/02/2026.
//
#pragma once

#include <GLFW/glfw3.h>
#include "Shader.h"

// Owns:
// - OpenGL calls
// - Shaders
// - Draw Calls

class Application;

class Renderer {

public:
    Renderer();
    void BeginFrame();
    void Draw(const Application& app);
    void EndFrame(GLFWwindow* window);

private:
    std::unique_ptr<Shader> shader = nullptr;
};


