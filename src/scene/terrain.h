#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "core/pairhash.h"
#include "noise/valuenoise.h"
#include "render/mesh.h"

const float TEMP_OFFSET = 44444.0f; // offset bo korzystamy tylko z jednego szumu
const float TEMP_SCALE = 512.0f;
const float H_OFFSET = -44444.0f;
const float H_SCALE = 128.0f;
const float TERRAIN_SCALE = 256.0f;

const float H_WATER = 0.33f;
const float H_MOUNT = 0.66f;
const float T_COLD  = 0.33f;
const float T_WARM  = 0.66f;
const float BLEND_RANGE = 0.1f;

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
    {90, 12}, {180, 18}, {30, 4},
    {80, 8}, {220, 14}, {40, 4},
    {100, 4}, {160, 20}, {20, 4}
};

static const char* biome_names[] = {
    "PLAINS", "MOUNTAINS", "WATER",
    "DESERT", "MESA", "WARM_WATER",
    "SNOW", "ICEBERG", "COLD_WATER"
};

const float blend_strength[9][9] = {
//           PLAI  MOUNT  WATER  DESER  MESA   WARM   SNOW   ICEB   COLD
/* PLAI */ { 0.0f, 1.5f,  1.0f,  2.0f,  1.5f,  1.0f,  1.5f,  1.0f,  1.0f },
/* MOUN */ { 1.5f, 0.0f,  1.0f,  1.5f,  2.0f,  1.0f,  2.0f,  2.0f,  1.0f },
/* WATE */ { 1.0f, 1.0f,  0.0f,  1.0f,  1.0f,  1.5f,  1.0f,  1.0f,  1.5f },
/* DESE */ { 2.0f, 1.5f,  1.0f,  0.0f,  2.0f,  1.0f,  1.0f,  1.0f,  1.0f },
/* MESA */ { 0.5f, 0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f },
/* WARM */ { 1.0f, 1.0f,  1.5f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.5f },
/* SNOW */ { 1.5f, 2.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  2.0f,  1.5f },
/* ICEB */ { 1.0f, 2.0f,  1.0f,  1.0f,  1.5f,  1.0f,  2.0f,  0.0f,  1.5f },
/* COLD */ { 1.0f, 1.0f,  1.5f,  1.0f,  1.0f,  1.5f,  1.5f,  1.5f,  0.0f },
};

struct TerrainGenerator {
    int seed;

    int t_alt;
    int h_alt;

    Mesh* terrain_mesh;
    static ValueNoiseGeneration v;

    std::unordered_map<std::pair<int, int>, Mesh, PairHash> active_chunks;

    TerrainGenerator();

    float get_temp_offset() {return TEMP_OFFSET;}
    float get_temp_scale() {return TEMP_SCALE;}
    float get_h_offset() {return H_OFFSET;}
    float get_h_scale() {return H_SCALE;}

    void set_altitudes(int t, int h) {t_alt = t; h_alt = h;}

    void resize_terrain();

    void init_value_noise();
    Mesh generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z);
    //unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50
    float get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset);
    BIOMES define_biome(float height, float temp);
    BIOMES get_current_biome(float x, float z);
    float get_blend(float h, float t, BIOMES& b1, BIOMES& b2);
};

#endif