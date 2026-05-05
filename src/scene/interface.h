#pragma once

#ifndef UI_H
#define UI_H 

#include <GLFW/glfw3.h>
#include <vector>
#include "imgui/imgui.h"

#include "minimap.h"

class AppPanel;
class MiniMap;

class GUI {
private:
    MiniMap* minimap;
    int pixels_per_chunk = 64;
    float map_window_size = 512.0f;
    ImVec2 offset = ImVec2(256, 256);

public:
    void init_gui(GLFWwindow* window, AppPanel* ap);
    void free_gui();

    void draw_gui(AppPanel* ap);
    void draw_screen(unsigned int texture_id);

    void draw_minimap(AppPanel* ap);
    void draw_diagram(AppPanel* ap);
};

#endif