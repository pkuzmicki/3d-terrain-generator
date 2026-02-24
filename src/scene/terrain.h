#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "noise/valuenoise.h"
#include "render/mesh.h"

#include <unordered_map>
#include <map>
#include <utility>

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

struct TerrainGenerator {
    Mesh* terrain_mesh;
    static ValueNoiseGeneration v;

    //std::map<std::pair<int, int>, TerrainChunk> chunks;
    std::unordered_map<std::pair<int, int>, Mesh, PairHash> active_chunks;

    TerrainGenerator();

    void resize_terrain();

    void init_value_noise();
    Mesh generate_value_noise_mesh(unsigned int size = 1000, unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50, int offset_x = 0, int offset_z = 0);
};

#endif