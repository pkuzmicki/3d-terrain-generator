#include "input.h"
#include "app_panel.h"

Input i;
static AppPanel* ap = &AppPanel::getInstance();

void init_input(GLFWwindow* window) {
    i.first_mouse = true;
    i.lastX = ap->SCREEN_WIDTH / 2.0f;
    i.lastY = ap->SCREEN_HEIGHT / 2.0f;
    i.camera = ap->renderer->main_camera;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (i.first_mouse) {
        i.lastX = xpos;
        i.lastY = ypos;
        i.first_mouse = false;
    }

    float xoffset = xpos - i.lastX;
    float yoffset = i.lastY - ypos;

    i.lastX = xpos;
    i.lastY = ypos;
    if (AppPanel::getInstance().is_mouse_hidden)
        i.camera->process_mouse_input(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (AppPanel::getInstance().is_mouse_hidden)
        i.camera->process_scroll_input(static_cast<float>(yoffset));
}

static bool lastTabState = false;

void key_callback(GLFWwindow* window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        i.camera->sprint = true;
    else
        i.camera->sprint = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        i.camera->process_keyboard_input(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        i.camera->process_keyboard_input(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        i.camera->process_keyboard_input(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        i.camera->process_keyboard_input(RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        i.camera->process_keyboard_input(UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        i.camera->process_keyboard_input(DOWN, delta_time);

    bool currentTabState = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;

    if (currentTabState && !lastTabState){
        ap->is_mouse_hidden = !AppPanel::getInstance().is_mouse_hidden;
        glfwSetCursorPos(window, ap->SCREEN_WIDTH/2, ap->SCREEN_HEIGHT/2);
    }

    lastTabState = currentTabState;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        i.camera->mode = SPECTATOR;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        i.camera->mode = OBJECT;
    
        
}