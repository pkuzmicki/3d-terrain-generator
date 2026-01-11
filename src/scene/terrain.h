#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include "noise/valuenoise.h"
#include "render/mesh.h"

struct TerrainGenerator {
    Mesh* terrain_mesh;
    static ValueNoiseGeneration v;

    TerrainGenerator();

    void resize_terrain();

    void init_value_noise();
    Mesh generate_value_noise_mesh(unsigned int size = 1000, unsigned int NUMOCTAVES = 8, unsigned int ALTITUDE = 50);
};

#endif