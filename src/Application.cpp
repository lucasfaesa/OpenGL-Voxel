#include "Application.h"
#include "Mesh.h"

Application::Application() {
    model = glm::mat4(1.0f);
}

void Application::Init() {

    mesh = std::make_unique<Mesh>();

    // 1. Fill the base floor (y=0) as you already did
    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            chunk.SetVoxel(x, 0, z, Voxels::SOLID_VOXEL);
        }
    }

    // 2. Build the pyramid layers starting from y=1
    for (int y = 1; y < 8; y++) {
        int start = y;          // The higher we go, the further in we start
        int end = 16 - y;       // The higher we go, the earlier we stop

        for (int x = start; x < end; x++) {
            for (int z = start; z < end; z++) {
                chunk.SetVoxel(x, y, z, Voxels::SOLID_VOXEL);
            }
        }
    }

    chunk.CreateMesh();
    mesh->UpdateData(chunk.GetVertices(), chunk.GetIndices());
}

void Application::ProcessInput(GLFWwindow *window, float deltaTime) {
    bool forward  = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool left     = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool right    = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool up       = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    bool down     = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;

    // Double-check this order: fwd, bwd, left, right, up, down
    camera.ProcessKeyboard(forward, backward, left, right, up, down, deltaTime);

    static double lastX = Engine::window_width/2;
    static double lastY = Engine::window_height/2;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    // If this is the first time we get mouse input,
    // set lastX/Y to the current position so the offset is 0
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y; // Reversed: y-coordinates range from bottom to top

    lastX = x;
    lastY = y;

    camera.ProcessMouse(xOffset, yOffset);
}

void Application::Update(float deltaTime) {
    //model = glm::mat4(1.0f);
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0,1,0));
}

void Application::Cleanup() {
    mesh.reset(); //triggering mesh destructor
}

const Camera &Application::GetCamera() const {
    return camera;
}
glm::mat4 Application::GetModelMatrix() const {
    return model;
}
Mesh *Application::GetMesh() const {
    return mesh.get();
}