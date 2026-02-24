#include "terrain.h"
#include "noise/valuenoise.h"
#include "render/renderer.h"

#include <vector>
#include <chrono>

ValueNoiseGeneration TerrainGenerator::v;

TerrainGenerator::TerrainGenerator() {
    init_value_noise();
}

float x;
float z;

void TerrainGenerator::init_value_noise() {
    //TODO potem do ustawienia w menu narazie na sztywno / raczej róźne biomy dodam poprostu na sztywno 
    const int POINTCOUNT = 31;
    int g_nUtahDistribution[POINTCOUNT] = {
    1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
    };

    int seed = std::time(0);
    std::srand(seed);

    x = (float)rand();
    z = (float)rand();

    v.Initialize();
    v.SetValueTable(g_nUtahDistribution, POINTCOUNT);  
}

Mesh TerrainGenerator::generate_value_noise_mesh(
    unsigned int chunk_size, 
    unsigned int numoctaves, 
    unsigned int altitude, 
    int offset_x, 
    int offset_z
) {
    float scale = 256.0f;

    std::vector<Vertex> positions(chunk_size*chunk_size);
    std::vector<float> heights(chunk_size*chunk_size);

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {

            float global_x = offset_x + i;
            float global_z = offset_z + j;

            float height = (altitude * v.GetHeight(
                global_x / scale, 
                global_z / scale, 
                0.5f, 
                2.0f, 
                numoctaves
            ));

            int index = i * chunk_size + j;

            positions[index].postion.x = global_x;
            positions[index].postion.y = height;
            positions[index].postion.z = global_z;
        }
    }

    int indices_count = chunk_size - 1;
    std::vector<unsigned int> indices(indices_count * indices_count * 6);
    int idx = 0;

    for (int z = 0; z < indices_count; z++) {
        for (int x = 0; x < indices_count; x++) {

            int topLeft = z * chunk_size + x;
            int topRight = z * chunk_size + x + 1;
            int bottomLeft = (z + 1) * chunk_size + x;
            int bottomRight = (z + 1) * chunk_size + x + 1;

            indices[idx++] = topLeft;
            indices[idx++] = bottomLeft;
            indices[idx++] = topRight;

            indices[idx++] = topRight;
            indices[idx++] = bottomLeft;
            indices[idx++] = bottomRight;
        }
    }

    return Mesh(positions, indices, {});
}
