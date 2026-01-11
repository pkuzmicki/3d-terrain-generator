#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "render/camera.h"
#include "render/mesh.h"
#include "terrain.h"

struct GeneratorSettings {
    // unsigned int width = 100;
    // unsigned int length = 100;
    unsigned int size = 1000;
    unsigned int altitude = 100;
    unsigned int numoctaves = 8;
};

class Scene {
public:
    std::vector<Mesh> meshes;

    virtual ~Scene() = default;
    
    virtual void update_scene();

    virtual GeneratorSettings* get_generator_settings() {return nullptr;}
};

class TerrainScene : public Scene {
    TerrainGenerator generator;
    GeneratorSettings settings;
public:
    TerrainScene();
    ~TerrainScene() override = default;

    void update_scene() override;
    GeneratorSettings* get_generator_settings() override {
        return &settings;
    }
};

class ModelScene : public Scene {};

#endif