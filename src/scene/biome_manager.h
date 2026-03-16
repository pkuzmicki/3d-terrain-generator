#pragma ONCE

#ifndef BIOME_MANAGER_H
#define BIOME_MANAGER_H

#include <vector>

enum BIOMES {
    PLAINS,
    DESERT,
    MOUNTAIS,
    COUNT
};

struct Biome {
    float scale;
    unsigned int altitude;
    unsigned int numoctaves;
};

struct BiomeSeed {
    float x;
    float z;
    BIOMES biome;
};

static Biome biome_values[] = {
    {256.0f, 100, 8},
    {65.0f, 10, 8},
    {256.0f, 250, 8}
};

std::vector<BiomeSeed> generate_biome_seeds(int chunk_x, int chunk_z, int chunk_size, int seed);
BIOMES get_biome(float x, float z, const std::vector<BiomeSeed>& seeds);

struct BiomeBlend {
    const BiomeSeed* nearest;
    const BiomeSeed* second;
    float w1; 
    float w2;
};

BiomeBlend get_biome_blend(float x, float z, const std::vector<BiomeSeed>& seeds);

#endif