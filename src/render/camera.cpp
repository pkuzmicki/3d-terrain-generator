#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

void Camera::updateCameraVectors() {
    if (mode == SPECTATOR) {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    } else if (mode == OBJECT) {
        float x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        float y = radius * sin(glm::radians(pitch));
        float z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        position = target + glm::vec3(x, y, z);
        this->front = glm::normalize(target + position);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
}

glm::mat4 Camera::get_view_matrix() {
    if (mode == SPECTATOR)
        return glm::lookAt(position, position + front, up);
        //return glm::lookAt(glm::vec3(0.0f, 250.0f, 0.0f), position + front, up);
    else {
        return glm::lookAt(position, target, up);
    }
}

void Camera::process_keyboard_input(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;
    if (sprint)
        velocity *= 25.0f;

    if (mode == SPECTATOR) {
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += up * velocity;
        if (direction == DOWN)
            position -= up * velocity;
    } else {
        if (direction == FORWARD)
            target.z += velocity;
        if (direction == BACKWARD)
            target.z -= velocity;
        if (direction == LEFT)
            target.x -= velocity;
        if (direction == RIGHT)
            target.x += velocity;
        if (direction == UP)
            target.y += velocity;
        if (direction == DOWN)
            target.y -= velocity;
        updateCameraVectors();
    }
    
}

void Camera::process_mouse_input(float xOffset, float yOffset, bool constrain_pitch) {
    xOffset *= mouse_sensitivity;
    yOffset *= mouse_sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrain_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::process_scroll_input(float yOffset) {
    zoom -= (float)yOffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}