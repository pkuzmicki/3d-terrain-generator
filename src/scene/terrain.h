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

const int flat_points = 32;
const int flat_distribution[flat_points] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

const int utah_points = 31;
const int utah_distribution[utah_points] = {
    1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
};

const int egypt_points = 16;
const int egypt_distribution[egypt_points] = {
    1, 7, 12, 13, 16, 27, 52, 41, 29, 23, 15, 9, 5, 3, 2, 1
};

const int alps_points = 25;
const int alps_distribution[alps_points] = {
  7, 6, 8, 8, 9, 10, 12, 12, 14, 15, 15, 16, 16, 16, 15, 14, 14, 13, 11, 9, 6, 4, 3, 2, 1
};

const int plains_points = 21;
const int plains_distribution[plains_points] = {
  26, 17, 17, 18, 18, 20, 21, 18, 16, 16, 16, 14, 12, 9, 5, 4, 3, 2, 2, 1, 1
};

//unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50  //domyslna wartosc 
static Biome biome_values[] = {
    {50, 12}, {100, 8}, {30, 4},
    {50, 8}, {100, 14}, {40, 4},
    {50, 4}, {100, 20}, {20, 4}
};

static const char* biome_names[] = {
    "PLAINS", "MOUNTAINS", "WATER",
    "DESERT", "MESA", "WARM_WATER",
    "SNOW", "ICEBERG", "COLD_WATER"
};

const float blend_strength[9][9] = {
//           PLAI  MOUNT  WATER  DESER  MESA   WARM   SNOW   ICEB   COLD
/* PLAI */ { 0.0f, 1.5f,  1.0f,  1.0f,  1.5f,  1.0f,  1.5f,  1.0f,  1.0f },
/* MOUN */ { 1.5f, 0.0f,  1.0f,  1.5f,  2.0f,  1.0f,  2.0f,  2.0f,  1.0f },
/* WATE */ { 1.0f, 1.0f,  0.0f,  1.0f,  1.0f,  1.5f,  1.0f,  1.0f,  1.5f },
/* DESE */ { 1.0f, 1.5f,  1.0f,  0.0f,  2.0f,  1.0f,  1.0f,  1.0f,  1.0f },
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
    //static ValueNoiseGeneration v;
    static ValueNoiseGeneration v_biomes[10];

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
    Mesh generate_value_noise_single_biome_mesh(unsigned int chunk_size, int offset_x, int offset_z);
    float get_point_height_for_biome(float x, float z, BIOMES b);
    float get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset);
    BIOMES define_biome(float height, float temp);
    BIOMES get_current_biome(float x, float z);
    float get_blend(float h, float t, BIOMES& b1, BIOMES& b2);
};

#endif