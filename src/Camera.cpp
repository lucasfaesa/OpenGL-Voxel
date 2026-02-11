#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>
#include <ostream>

Camera::Camera()
{
    position = {0,2,0};
    front = {0,0,-1};
    worldUp = {0,1,0};

    yaw = 45.0f;
    pitch = 0.0f;
    movementSpeed = 3.0f;

    UpdateCameraVectors();
}

//eye = camera position
//center = where the camera is looking
//up = orientation reference

//glm::lookAt is the function that creates your View Matrix
//It is essentialy the "Director" that tells the world how to position
//itself so it looks like you are standin in a specific spot
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) const
{
    return glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}

void Camera::ProcessKeyboard(bool forward, bool backward, bool left, bool right, bool up, bool down, float deltaTime) {
    const float velocity = movementSpeed * deltaTime;

    if (forward)
        position += front * velocity;
    if (backward)
        position -= front * velocity;

    // Use 'this->right' to be explicit
    if (left)
        position -= this->right * velocity;
    if (right)
        position += this->right * velocity;

    // Use 'this->up' for vertical movement
    if (up)
        position += this->up * velocity;
    if (down)
        position -= this->up * velocity;
}

void Camera::UpdateCameraVectors(){
    
    glm::vec3 f;
    
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessMouse(float dx, float dy)
{
    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch += dy;

    pitch = std::clamp(pitch, minPitch, maxPitch);

    UpdateCameraVectors();
}

