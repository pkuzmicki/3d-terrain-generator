#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;

class SceneManager {
public:
    Scene* current_scene;

    void save_scene();
    void discard_scene();
};

#endif