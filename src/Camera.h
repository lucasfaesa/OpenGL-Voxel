#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
    public:
    //Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    //Euler angles
    float Yaw;
    float Pitch;

    //movement
    float MovementSpeed;

    //Constructor
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    //Returns the view matrix
    glm::mat4 GetViewMatrix() const;

    //Processes keyboard input
    void ProcessKeyboard(bool forward, bool backward, bool left, bool right, bool up, bool down, float deltaTime);
    void ProcessMouse(const double& xpos, const double& ypos);
    private:
    void UpdateCameraVectors();

    double lastMouseX;
    double lastMouseY;
    double deltaX;
    double deltaY;
    bool firstMouse = true;
    static constexpr float sensitivity = 0.1f;
};