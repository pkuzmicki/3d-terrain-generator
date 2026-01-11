#ifndef APP_PANEL_H
#define APP_PANEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render/renderer.h"

class Renderer;

class AppPanel {
protected:
    AppPanel() = default;

public:
    AppPanel(AppPanel &other) = delete;
    void operator=(const AppPanel &) = delete;
    static AppPanel& getInstance();

    bool is_mouse_hidden = false;

    GLFWwindow* window;
    Renderer* renderer;
    SceneManager* s_manager;

    const unsigned int SCREEN_WIDTH = 1024;
    const unsigned int SCREEN_HEIGHT = 600;

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    void init();
    void run();
    void update_mode();
};

#endif