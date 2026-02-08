#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
    Camera();

    //Returns the view matrix
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect) const;

    //Processes keyboard input
    void ProcessKeyboard(bool forward, bool backward, bool left, bool right, bool up, bool down, float deltaTime);
    void ProcessMouse(float dx, float dy);

private:
    void UpdateCameraVectors();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //Euler angles
    float yaw;
    float pitch;

    float movementSpeed;

    static constexpr float sensitivity = 0.1f;
    static constexpr float minPitch = -89.0f;
    static constexpr float maxPitch = 89.0f;
    static constexpr float fov = 45.0f;
    static constexpr float near_plane = 0.1f;
    static constexpr float far_plane = 100.0f;
};
