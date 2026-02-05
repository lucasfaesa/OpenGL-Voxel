#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
                : Position(position),WorldUp(up), Yaw(yaw), Pitch(pitch), MovementSpeed(2.5f)
{
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    UpdateCameraVectors();
}


//eye = camera position
//center = where the camera is looking
//up = orientation reference

//glm::lookAt is the function that creates your View Matrix
//It is essentialy the "Director" that tells the world how to position
//itself so it looks like you are standin in a specific spot
glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(Position, Position+Front, Up);
}

void Camera::ProcessKeyboard(bool forward, bool backward, bool left, bool right, float deltaTime){
    float velocity = MovementSpeed * deltaTime;

    if(forward)
        Position += Front * velocity;
    if(backward)
        Position -= Front * velocity;
    if(left)
        Position -= Right * velocity;
    if(right)
        Position += Right * velocity;
    }

void Camera::UpdateCameraVectors(){
    
    glm::vec3 front;
    
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}