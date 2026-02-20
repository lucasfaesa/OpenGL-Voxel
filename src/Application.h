#pragma once
#include "Camera.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Engine.h"
#include "World.h"

class Application
{
public:
    Application();
    
    void Init();

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void Update(float deltaTime);

    const Camera& GetCamera() const;
    glm::mat4 GetModelMatrix() const;
    Mesh* GetMesh() const;
    World& GetWorld();
private:
    Camera camera;
    World world;
    bool firstMouse = true;
    ChunkPos lastChunkPosPlayerWasIn;
};