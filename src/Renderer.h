//
// Created by lucas on 07/02/2026.
//
#pragma once

#include <GLFW/glfw3.h>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "World.h"

class Application;

class Renderer {

public:
    Renderer();
    ~Renderer();
    void BeginFrame();
    void Draw(World& world, const Camera& camera);
    void EndFrame(GLFWwindow* window);

private:
    std::unique_ptr<Shader> shader = nullptr;
    std::unique_ptr<Shader> skyShader = nullptr;

    Frustum frustum_;
    Texture grass;

    unsigned int skyboxVAO = 0;
    unsigned int skyboxVBO = 0;

    void SetupSkybox();
};