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

    ~Application() {
        delete mesh;
    }

private:
    Camera camera;
    glm::mat4 model {};
    Mesh* mesh {nullptr};
    bool firstMouse = true;
};