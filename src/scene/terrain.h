#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "core/pairhash.h"
#include "noise/valuenoise.h"
#include "render/mesh.h"
#include "scene/biome_manager.h"

enum BIOMES {
    PLAINS, MOUNTAIS, WATER,
    DESERT, MESA, WARM_WATER,
    SNOW, ICEBEARG, COLD_WATER,
    COUNT
};

struct Biome {
    unsigned int altitude;
    unsigned int numoctaves;
};

static Biome biome_values[] = {
    {100, 8}, {150, 14}, {30, 4},
    {90, 3}, {210, 14}, {40, 4},
    {110, 5}, {180, 14}, {20, 4}
};

static const char* biome_names[] = {
    "PLAINS", "MOUNTAINS", "WATER",
    "DESERT", "MESA", "WARM_WATER",
    "SNOW", "ICEBERG", "COLD_WATER"
};

struct TerrainGenerator {
    int seed;

    Mesh* terrain_mesh;
    static ValueNoiseGeneration v;

    std::unordered_map<std::pair<int, int>, Mesh, PairHash> active_chunks;

    TerrainGenerator();

    void resize_terrain();

    void init_value_noise();
    Mesh generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z);
    //unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50
    float get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset);
    BIOMES define_biome(float height, float temp);
    BIOMES get_current_biome(float x, float z);

};

#endif