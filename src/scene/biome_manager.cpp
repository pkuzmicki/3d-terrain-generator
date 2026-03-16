#include "biome_manager.h"
#include "noise/jc_voronoi.h"

#include <algorithm>

unsigned int biome_hash(int x, int z, unsigned int seed) {
    unsigned int h = x * 213769420 + z * 420692137;
    return h ^ seed;
}

// std::vector<BiomeSeed> generate_biome_seeds(int chunk_x, int chunk_z, int chunk_size, int seed) {
//     const int biome_size = 5;

//     std::vector<BiomeSeed> seeds;

//     for (int i = chunk_x - biome_size; i <= chunk_x + biome_size; i++) {
//         for (int j = chunk_z - biome_size; j <= chunk_z + biome_size; j++) {

//             // int x = chunk_x + i;
//             // int z = chunk_z + j;

//             unsigned int h = biome_hash(i, j, seed);

//             BiomeSeed s;

//             s.x = i * chunk_size + (h % chunk_size);
//             s.z = j * chunk_size + ((h>>8) % chunk_size);
//             s.biome = (BIOMES)(h % BIOMES::COUNT);

//             seeds.push_back(s);
//         } 
//     }
//     return seeds;
// }

std::vector<BiomeSeed> generate_biome_seeds(int chunk_x, int chunk_z, int chunk_size, int seed) {
    const int biome_grid_step = 4;  // co ile chunków jeden seed (zwiększ dla większych biomów)
    const int biome_radius = 3;     // ile gridcelli w każdą stronę (było: biome_size chunków)

    std::vector<BiomeSeed> seeds;

    int grid_x = (int)std::floor((float)chunk_x / biome_grid_step);
    int grid_z = (int)std::floor((float)chunk_z / biome_grid_step);

    for (int i = grid_x - biome_radius; i <= grid_x + biome_radius; i++) {
        for (int j = grid_z - biome_radius; j <= grid_z + biome_radius; j++) {

            unsigned int h = biome_hash(i, j, seed);

            BiomeSeed s;
            int cell_size = biome_grid_step * chunk_size;
            s.x = i * cell_size + (h % cell_size);
            s.z = j * cell_size + ((h >> 8) % cell_size);
            s.biome = (BIOMES)(h % BIOMES::COUNT);

            seeds.push_back(s);
        }
    }
    return seeds;
}

BIOMES get_biome(float x, float z, const std::vector<BiomeSeed>& seeds) {
    float min_dist = std::numeric_limits<float>::max();
    BIOMES biome = BIOMES::PLAINS;
    for (const auto& s : seeds) {
        float dx = x - s.x;
        float dz = z - s.z;

        float d = dx * dx + dz * dz;

        if (d < min_dist) {
            min_dist = d;
            biome = s.biome;
        }
    }
    return biome;
}

//test

BiomeBlend get_biome_blend(float x, float z, const std::vector<BiomeSeed>& seeds) {
    const BiomeSeed* nearest = nullptr;
    const BiomeSeed* second  = nullptr;
    float min1 = std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max();

    for (const auto& s : seeds) {
        float dx = x - s.x, dz = z - s.z;
        float d2 = dx*dx + dz*dz;
        if (d2 < min1) { min2=min1; second=nearest; min1=d2; nearest=&s; }
        else if (d2 < min2) { min2=d2; second=&s; }
    }

    float d1 = std::sqrt(min1), d2 = std::sqrt(min2);
    float total = d1 + d2;
    float t = (total > 1e-5f) ? (d2 - d1) / total : 1.0f;

    const float blend_range = 0.3f;
    t = std::clamp((t + blend_range) / (2.0f * blend_range), 0.0f, 1.0f);
    t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);

    return { nearest, second, t, 1.0f - t };
}
