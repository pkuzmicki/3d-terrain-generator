#include "terrain.h"
#include "noise/valuenoise.h"
#include "render/renderer.h"

#include <vector>
#include <chrono>

ValueNoiseGeneration TerrainGenerator::v;

TerrainGenerator::TerrainGenerator() {
    init_value_noise();
}

void TerrainGenerator::init_value_noise() {
    //TODO potem do ustawienia w menu narazie na sztywno
    const int POINTCOUNT = 31;
    int g_nUtahDistribution[POINTCOUNT] = {
    1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
    };

    v.Initialize();
    v.SetValueTable(g_nUtahDistribution, POINTCOUNT);  
}

Mesh TerrainGenerator::generate_value_noise_mesh(unsigned int size, unsigned int numoctaves, unsigned int altitude) {
    // const int NUMOCTAVES = 8;
    // const int ALTITUDE = 100;
    unsigned int terrain_width = size;
    unsigned int terrain_length = size;

    int seed = std::time(0);
    std::srand(seed);

    float x = (float)rand();
    float z = (float)rand();
    std::vector<float> heights(terrain_width * terrain_length);

    for (int i = 0; i < terrain_width; i++) {
        for (int j = 0; j < terrain_length; j++) {
            heights[i * terrain_width + j] = (altitude * v.GetHeight(x + i/256.0f, z + j/256.0f, 0.5f, 2.0f, numoctaves));
        }
    }

    std::vector<Vertex> positions(terrain_width * terrain_length);

    int i1 = 0;

    for (int z = 0; z < terrain_width; z++) {
        for (int x = 0; x < terrain_length; x++) {
            float x1 = 0.2f * x;
            float y1 = heights[z * terrain_length + x];
            float z1 = 0.2f * z;

            positions[i1].postion.x = x1;
            positions[i1].postion.y = y1;
            positions[i1].postion.z = z1;
            i1++;
        }
    }

    // SUBSTYTUT ZROB INNE
    std::vector<unsigned int> indices((terrain_width - 1) * (terrain_length - 1) * 6);

    int idx = 0;

    for (int z = 0; z < terrain_width - 1; z++) {
        for (int x = 0; x < terrain_length - 1; x++) {
            int topLeft     =  z      * terrain_length + x;
            int topRight    =  z      * terrain_length + x + 1;
            int bottomLeft  = (z + 1) * terrain_length + x;
            int bottomRight = (z + 1) * terrain_length + x + 1;

            // trójkąt 1
            indices[idx++] = topLeft;
            indices[idx++] = bottomLeft;
            indices[idx++] = topRight;

            // trójąt 2
            indices[idx++] = topRight;
            indices[idx++] = bottomLeft;
            indices[idx++] = bottomRight;
        }
    }

    return Mesh(positions, indices, {});
}
