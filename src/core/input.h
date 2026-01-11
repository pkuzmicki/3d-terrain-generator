#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <glfw/glfw3.h>
#include "app_panel.h"

class Input {
public:
    bool first_mouse;
    float lastX;
    float lastY;
    Camera* camera;
};

void init_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, float delta_time);

#endif