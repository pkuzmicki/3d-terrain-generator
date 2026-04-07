#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cmath>

#include "core/app_panel.h"
#include "render/camera.h"
#include "render/mesh.h"
#include "terrain.h"
//#include "scene/biome_manager.h"

struct GeneratorSettings {
    unsigned int chunk_size = 32;
    unsigned int altitude = 100;
    unsigned int numoctaves = 8;
    unsigned int render_distance = 10;

    // const int NUMOCTAVES = 8;
    // const int ALTITUDE = 100;
};

class Scene {
public:
    std::vector<Mesh> meshes;

    virtual ~Scene() = default;
    
    virtual void update_scene();

    virtual void add_chunk(std::pair<int, int> coords);

    virtual GeneratorSettings* get_generator_settings() {return nullptr;}
    virtual TerrainGenerator* get_generator() {return nullptr;}
};

class TerrainScene : public Scene {
    TerrainGenerator generator;
    GeneratorSettings settings;
public:
    TerrainScene();
    ~TerrainScene() override = default;

    void update_scene() override;
    void add_chunk(std::pair<int, int> coords) override;
    GeneratorSettings* get_generator_settings() override {
        return &settings;
    }

    TerrainGenerator* get_generator() override {
        return &generator;
    }
};

class ModelScene : public Scene {};

#endif