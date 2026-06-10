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

// const int utah_points = 31;
// const int utah_distribution[utah_points] = {
//     1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
// };

const int flat_points = 32;
const int flat_distribution[flat_points] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

//PLAINS
const int stepy_points = 32;
const int stepy_distribution[stepy_points] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 2, 3, 4, 6, 10, 11, 15, 19, 29, 22, 19, 18, 18, 16, 16, 18, 8, 2
};
//MOUNTAIS
const int everest_points = 31;
const int everest_distribution[everest_points] = {
  2, 3, 1, 2, 2, 3, 4, 5, 5, 9, 13, 12, 12, 14, 13, 14, 14, 14, 15, 14, 14, 13, 13, 14, 8, 7, 6, 4, 3, 2, 1
};
//WATER
const int baltyk_points = 30;
const int baltyk_distribution[baltyk_points] = {
  16, 15, 17, 7, 15, 19, 20, 8, 20, 18, 9, 16, 18, 11, 6, 8, 5, 3, 5, 4, 2, 1, 3, 5, 1, 1, 2, 0, 0, 1
};
//SNOW
const int snow_points = 32;
const int snow_distribution[snow_points] = {
  1, 1, 2, 3, 4, 5, 5, 5, 6, 6, 6, 7, 10, 10, 14, 15, 13, 12, 13, 13, 13, 14, 12, 11, 11, 9, 9, 9, 8, 5, 3, 1
};
//ICEBERG
const int fiordy_points = 27;
const int fiordy_distribution[fiordy_points] = {
  4, 5, 6, 7, 9, 11, 13, 16, 17, 17, 17, 16, 15, 14, 14, 13, 13, 10, 9, 7, 7, 5, 4, 3, 2, 1, 1
};
//COLD_WATER
const int norweskie_points = 24;
const int norweskie_distribution[norweskie_points] = {
  8, 13, 11, 9, 8, 9, 9, 9, 10, 14, 16, 15, 13, 18, 23, 18, 18, 13, 7, 4, 4, 4, 2, 1
};
//DESEART
const int sahara_points = 24;
const int sahara_distribution[sahara_points] = {
  1, 1, 1, 8, 15, 20, 23, 22, 24, 21, 23, 17, 15, 13, 12, 9, 9, 6, 5, 4, 3, 2, 1, 1
};
//MESA
const int canyon_points = 27;
const int canyon_distribution[canyon_points] = {
  1, 2, 4, 5, 7, 12, 15, 22, 23, 22, 20, 18, 15, 11, 7, 8, 11, 10, 11, 8, 6, 5, 4, 3, 3, 2, 1
};
//WARM_WATER
const int srodziemne_points = 26;
const int srodziemne_distribution[srodziemne_points] = {
  3, 3, 4, 4, 6, 9, 13, 15, 16, 16, 15, 15, 13, 15, 16, 16, 14, 13, 13, 11, 8, 6, 5, 4, 2, 1
};


enum BIOMES {
    PLAINS = 0, MOUNTAIS = 1, WATER = 2,
    DESERT = 3, MESA = 4, WARM_WATER = 5,
    SNOW = 6, ICEBEARG = 7, COLD_WATER = 8,
    COUNT = 9
};

struct Biome {
    int altitude;
    unsigned int numoctaves;
};

static const char* biome_names[] = {
    "PLAINS", "MOUNTAINS", "WATER",
    "DESERT", "MESA", "WARM_WATER",
    "SNOW", "ICEBERG", "COLD_WATER"
};

const float blend_strength[9][9] = {
//           PLAI  MOUNT  WATER  DESER  MESA   WARM   SNOW   ICEB   COLD
/* PLAI */ { 0.0f, 1.5f,  1.0f,  1.5f,  1.5f,  1.0f,  1.5f,  1.0f,  1.0f },
/* MOUN */ { 1.5f, 0.0f,  1.0f,  1.5f,  2.0f,  1.0f,  2.0f,  2.0f,  1.0f },
/* WATE */ { 1.0f, 1.0f,  0.0f,  1.0f,  1.0f,  1.5f,  1.0f,  1.0f,  1.5f },
/* DESE */ { 1.5f, 1.5f,  1.0f,  0.0f,  2.0f,  1.0f,  1.0f,  1.0f,  1.0f },
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

    //unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50  //default vals
    // Biome biome_values[9] = {
    //     {50, 12}, {50, 12}, {-20, 7},
    //     {50, 8}, {70, 14}, {-20, 7},
    //     {50, 4}, {70, 20}, {-20, 7}
    // };

    Biome biome_values[9] = {
      {55, 15}, {170, 25}, {-50, 10},
      {65, 12}, {130, 20}, {-25, 20},
      {45, 12}, {110, 20}, {-100, 16}
    };

    TerrainGenerator();

    float get_temp_offset() {return TEMP_OFFSET;}
    float get_temp_scale() {return TEMP_SCALE;}
    float get_h_offset() {return H_OFFSET;}
    float get_h_scale() {return H_SCALE;}

    void set_altitudes(int t, int h) {t_alt = t; h_alt = h;}

    void resize_terrain();

    void init_value_noise();
    Mesh generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z);
    Mesh generate_value_noise_single_biome_mesh(unsigned int chunk_size, int offset_x, int offset_z, unsigned int biome_index);
    float get_point_height_for_biome(float x, float z, BIOMES b);
    float get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset);
    BIOMES define_biome(float height, float temp);
    BIOMES get_current_biome(float x, float z);
    float get_blend(float h, float t, BIOMES& b1, BIOMES& b2);

    float smoothstep(float edge0, float edge1, float x);
};

#endif