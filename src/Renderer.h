//
// Created by lucas on 07/02/2026.
//
#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "World.h"

// Owns:
// - OpenGL calls
// - Shaders
// - Draw Calls

class Application;

class Renderer {

public:
    Renderer();
    void BeginFrame();
    void Draw(World& world, const Camera& camera);
    void EndFrame(GLFWwindow* window);

private:
    std::unique_ptr<Shader> shader = nullptr;
    Frustum frustum_;
    Texture grass;
};


