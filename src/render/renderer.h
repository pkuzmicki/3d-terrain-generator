#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "scene/scene_manager.h"
#include "scene/scene.h"

class Camera;

class Renderer {
private:
    Camera* main_camera;
    Shader shader;
    glm::mat4 projection;

public:
    Camera* get_cam();

    Renderer();
    void calc_matrix();
    void render_scene(Scene &scene);
    void frame_begin();

    unsigned int screen_texture;

    unsigned int FBO, RBO;
    void create_framebuffer();
    void bind_framebuffer();
    void unbind_framebuffer();

    Shader get_shader() {return shader;}
};

#endif