#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "scene/scene_manager.h"
#include "scene/scene.h"

class Camera;

class Renderer {
public:
    Camera* main_camera;

    Shader shader;

    glm::mat4 projection;

    Renderer();
    void calc_matrix();
    void render_scene(Scene &scene);
    void frame_begin();
};

#endif