#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Engine.h"

class Application
{
public:
    Application();
    
    // New: Handles OpenGL resource creation after GLAD is ready
    void Init(); 

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void Update(float deltaTime);

    const Camera& GetCamera() const;
    glm::mat4 GetModelMatrix() const;
    Mesh* GetMesh() const;

private:
    Camera camera;
    glm::mat4 model {};
    std::unique_ptr<Mesh> mesh = nullptr;
    bool firstMouse = true;
};