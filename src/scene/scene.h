#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cmath>

#include "core/app_panel.h"
#include "render/camera.h"
#include "render/mesh.h"
#include "terrain.h"
#include "interface.h"

class AppPanel;
class MiniMap;

struct GeneratorSettings {
    int world_seed;

    unsigned int chunk_size = 32;
    unsigned int altitude = 100;
    unsigned int numoctaves = 8;
    unsigned int render_distance = 1;

    MAP_MODE mode = MAP_MODE::BIOME;

    unsigned int temp_alt = 3;
    unsigned int height_alt = 10;

    const float TEMP_OFFSET = 100.0f; // offset bo korzystamy tylko z jednego szumu
    const float TEMP_SCALE = 512.0f;
    const float H_OFFSET = -100.0f;
    const float H_SCALE = 128.0f;
    const float TERRAIN_SCALE = 256.0f;


    bool is_in_single_biome_mode = false;
    unsigned int biome_index = 0;

    bool use_light = true;
};

class Scene {
public:
    //std::vector<Mesh> meshes;

    virtual ~Scene() = default;
    
    virtual void update_scene();

    virtual void add_chunk(std::pair<int, int> coords);

    virtual GeneratorSettings* get_generator_settings() {return nullptr;}
    virtual TerrainGenerator* get_generator() {return nullptr;}
    virtual MiniMap* get_minimap() {return nullptr;}
};

class TerrainScene : public Scene {
private:
    AppPanel* ap;
    TerrainGenerator generator;
    GeneratorSettings settings;
    MiniMap* minimap;
public:
    TerrainScene(AppPanel* ap);
    ~TerrainScene() override = default;

    void update_scene() override;
    void add_chunk(std::pair<int, int> coords) override;

    GeneratorSettings* get_generator_settings() override {
        return &settings;
    }

    TerrainGenerator* get_generator() override {
        return &generator;
    }

    MiniMap* get_minimap() override {
        return minimap;
    }
};

class ModelScene : public Scene {};

#endif