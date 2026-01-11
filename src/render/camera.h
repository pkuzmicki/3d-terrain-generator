#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/app_panel.h"

enum Camera_Mode {
    SPECTATOR,
    OBJECT
};

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float zoom = 45.0f;

class Camera {
private:
    void updateCameraVectors();

public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    glm::vec3 target;
    float radius;

    float yaw;
    float pitch;
    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    Camera_Mode mode = SPECTATOR;
    bool sprint = false;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH): 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(zoom) {
            this->position = position;
            this->world_up = up;
            this->yaw = yaw;
            this->pitch = pitch;

            this->target = position;
            this->radius = 3.0f;
            updateCameraVectors();
        }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
        front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(zoom) {
            position = glm::vec3(posX, posY, posZ);
            up = glm::vec3(upX, upY, upZ);
            yaw = yaw;
            pitch = pitch;
            updateCameraVectors();
        }

    glm::mat4 get_view_matrix();

    void process_keyboard_input(Camera_Movement direction, float deltaTime);

    void process_mouse_input(float xOffset, float yOffset, bool constrain_pitch = true);

    void process_scroll_input(float yOffset);

};

#endif