#pragma once
#include "Camera.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Engine.h"

class Application
{
public:
    Application();
    
    void Init();

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void Update(float deltaTime);
    void Cleanup();

    const Camera& GetCamera() const;
    glm::mat4 GetModelMatrix() const;
    Mesh* GetMesh() const;

private:
    Camera camera;
    glm::mat4 model {};
    std::unique_ptr<Mesh> mesh = nullptr;
    bool firstMouse = true;

    Chunk chunk;
};