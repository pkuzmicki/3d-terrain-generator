#include "terrain.h"
#include "noise/valuenoise.h"
#include "render/renderer.h"

#include <vector>
#include <chrono>
#include <algorithm>

const float TEMP_OFFSET = 44444.0f; // offset bo korzystamy tylko z jednego szumu
const float TEMP_SCALE = 512.0f;
const float H_OFFSET = -44444.0f;
const float H_SCALE = 128.0f;
const float TERRAIN_SCALE = 256.0f;

ValueNoiseGeneration TerrainGenerator::v;

TerrainGenerator::TerrainGenerator() {
    init_value_noise();
}

float x;
float z;

void TerrainGenerator::init_value_noise() {
    const int POINTCOUNT = 31;
    int g_nUtahDistribution[POINTCOUNT] = {
    1, 4, 6, 7, 7, 8, 10, 11, 14, 30, 37, 30, 19, 11, 8, 5, 5, 4, 3, 3, 3, 3, 3, 3, 5, 4, 4, 3, 2, 2, 1
    };

    seed = std::time(0);
    std::srand(seed);
    std::cout<<"seed: "<<seed<<"\n";

    x = (float)rand();
    z = (float)rand();

    v.Initialize();
    v.SetValueTable(g_nUtahDistribution, POINTCOUNT);  
}

// Mesh TerrainGenerator::generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z) {
//     std::vector<Vertex> positions(chunk_size*chunk_size);
//     std::vector<float> heights(chunk_size*chunk_size);

//     for (int i = 0; i < chunk_size; i++) {
//         for (int j = 0; j < chunk_size; j++) {

//             float global_x = offset_x + i;
//             float global_z = offset_z + j;

//             float t = get_point_height(global_x, global_z, 3, TEMP_SCALE, TEMP_OFFSET);
//             float h = get_point_height(global_x, global_z, 4, H_SCALE, H_OFFSET);
//             BIOMES b = define_biome(h, t);

//             float height = biome_values[b].altitude * get_point_height(global_x, global_z, biome_values[b].numoctaves, TERRAIN_SCALE, 0.0f);

//             int index = i * chunk_size + j;

//             positions[index].postion.x = global_x;
//             positions[index].postion.y = height;
//             positions[index].postion.z = global_z;
//         }
//     }

//     int indices_count = chunk_size - 1;
//     std::vector<unsigned int> indices(indices_count * indices_count * 6);
//     int idx = 0;

//     for (int z = 0; z < indices_count; z++) {
//         for (int x = 0; x < indices_count; x++) {

//             int topLeft = z * chunk_size + x;
//             int topRight = z * chunk_size + x + 1;
//             int bottomLeft = (z + 1) * chunk_size + x;
//             int bottomRight = (z + 1) * chunk_size + x + 1;

//             indices[idx++] = topLeft;
//             indices[idx++] = bottomLeft;
//             indices[idx++] = topRight;

//             indices[idx++] = topRight;
//             indices[idx++] = bottomLeft;
//             indices[idx++] = bottomRight;
//         }
//     }

//     return Mesh(positions, indices, {});
// }


Mesh TerrainGenerator::generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z) {
    std::vector<Vertex> positions(chunk_size*chunk_size);

    const float BLEND_RANGE = 0.1f;
    const float H_WATER = 0.33f;
    const float H_MOUNT = 0.66f;
    const float T_COLD  = 0.33f;
    const float T_WARM  = 0.66f;

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {

            float global_x = offset_x + i;
            float global_z = offset_z + j;

            float t = get_point_height(global_x, global_z, 3, TEMP_SCALE, TEMP_OFFSET);
            float h = get_point_height(global_x, global_z, 4, H_SCALE, H_OFFSET);

            // znajdz najblizszy prog i odleglosc do niego (ze znakiem)
            struct Threshold { float value; bool is_height; };
            Threshold thresholds[4] = {
                { H_WATER, true  },
                { H_MOUNT, true  },
                { T_COLD,  false },
                { T_WARM,  false }
            };

            float closest_dist = 1.0f;
            Threshold* nearest = nullptr;
            for (auto& th : thresholds) {
                float val = th.is_height ? h : t;
                float dist = std::abs(val - th.value);
                if (dist < closest_dist) {
                    closest_dist = dist;
                    nearest = &th;
                }
            }

            float height;

            if (closest_dist > BLEND_RANGE) {
                BIOMES b = define_biome(h, t);
                height = biome_values[b].altitude * get_point_height(
                    global_x, global_z, biome_values[b].numoctaves, TERRAIN_SCALE, 0.0f
                );
            } else {
                // b1 zawsze po nizszej stronie progu, b2 po wyzszej
                // dzieki temu po obu stronach progu mamy te same dwa biomy
                BIOMES b1, b2;
                float signed_dist; // ujemny = ponizej progu, dodatni = powyzej

                if (nearest->is_height) {
                    b1 = define_biome(nearest->value - 0.01f, t);
                    b2 = define_biome(nearest->value + 0.01f, t);
                    signed_dist = h - nearest->value;
                } else {
                    b1 = define_biome(h, nearest->value - 0.01f);
                    b2 = define_biome(h, nearest->value + 0.01f);
                    signed_dist = t - nearest->value;
                }

                // alpha: 0.0 = daleko ponizej (tylko b1)
                //        0.5 = dokladnie na progu (50/50)
                //        1.0 = daleko powyzej (tylko b2)
                float alpha = (signed_dist / BLEND_RANGE) * 0.5f + 0.5f;
                alpha = std::clamp(alpha, 0.0f, 1.0f);
                alpha = alpha * alpha * (3.0f - 2.0f * alpha); // smoothstep

                float h1 = biome_values[b1].altitude * get_point_height(
                    global_x, global_z, biome_values[b1].numoctaves, TERRAIN_SCALE, 0.0f
                );
                float h2 = biome_values[b2].altitude * get_point_height(
                    global_x, global_z, biome_values[b2].numoctaves, TERRAIN_SCALE, 0.0f
                );

                height = h1 * (1.0f - alpha) + h2 * alpha;
            }

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
            int topLeft     = z * chunk_size + x;
            int topRight    = z * chunk_size + x + 1;
            int bottomLeft  = (z + 1) * chunk_size + x;
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

float TerrainGenerator::get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset) {
    float global_x = x / scale + offset;
    float global_z = z / scale + offset;

    float height = v.GetHeight(global_x, global_z, 0.5f, 2.0f, numoctaves);
    //std::cout<<height<<"\n";
    return height;
}

BIOMES TerrainGenerator::define_biome(float height, float temp) {
    if (height < 0.33f) {
        if (temp < 0.33f) return BIOMES::COLD_WATER;
        if (temp < 0.66f) return BIOMES::WATER;
        return BIOMES::WARM_WATER;
    } 
    if (height < 0.66f) {
        if (temp < 0.33f) return BIOMES::SNOW;
        if (temp < 0.66f) return BIOMES::PLAINS;
        return BIOMES::DESERT;
    } 
    if (temp < 0.33f) return BIOMES::ICEBEARG;
    if (temp < 0.66f) return BIOMES::MOUNTAIS;
    return BIOMES::MESA;
}

BIOMES TerrainGenerator::get_current_biome(float x, float z) {
    float t = get_point_height(x, z, 3, TEMP_SCALE, TEMP_OFFSET);
    float h = get_point_height(x, z, 4, H_SCALE, H_OFFSET);
    return define_biome(h, t);
}

// float TerrainGenerator::getBiomeInterp(float h, float t, BIOMES& b1, BIOMES& b2) {
//     const float BLEND_RANGE = 0.08f; // szerokość strefy przejściowej

//     b1 = define_biome(h, t);

//     // progi wysokości i temperatury
//     const float H_WATER = 0.33f;
//     const float H_MOUNT = 0.63f;
//     const float T_COLD  = 0.33f;
//     const float T_WARM  = 0.66f;

//     // znajdź najbliższy próg i odległość do niego
//     float closest_dist = 1.0f;
//     float nearest_threshold = 0.0f;
//     bool  is_height_threshold = true;

//     auto check = [&](float val, float threshold, bool is_h) {
//         float dist = std::abs(val - threshold);
//         if (dist < closest_dist) {
//             closest_dist = dist;
//             nearest_threshold = threshold;
//             is_height_threshold = is_h;
//         }
//     };

//     check(h, H_WATER, true);
//     check(h, H_MOUNT, true);
//     check(t, T_COLD,  false);
//     check(t, T_WARM,  false);

//     // daleko od granicy — brak blendowania
//     if (closest_dist > BLEND_RANGE) {
//         b2 = b1;
//         return 0.0f;
//     }

//     // znajdź biom po drugiej stronie progu
//     if (is_height_threshold) {
//         float other_h = h < nearest_threshold
//             ? nearest_threshold + 0.01f
//             : nearest_threshold - 0.01f;
//         b2 = define_biome(other_h, t);
//     } else {
//         float other_t = t < nearest_threshold
//             ? nearest_threshold + 0.01f
//             : nearest_threshold - 0.01f;
//         b2 = define_biome(h, other_t);
//     }

//     // smooth interpolacja (smoothstep zamiast liniowej)
//     float alpha = closest_dist / BLEND_RANGE; // 0 = na granicy, 1 = daleko
//     alpha = 1.0f - alpha;                     // 0 = daleko, 1 = na granicy
//     alpha = alpha * alpha * (3.0f - 2.0f * alpha); // smoothstep

//     return alpha;
// }

// Mesh TerrainGenerator::generate_value_noise_mesh1(
//     unsigned int chunk_size, 
//     unsigned int numoctaves, 
//     unsigned int altitude, 
//     int offset_x, 
//     int offset_z
// ) {
//     float scale = 256.0f;

//     std::vector<Vertex> positions(chunk_size*chunk_size);
//     std::vector<float> heights(chunk_size*chunk_size);

//     for (int i = 0; i < chunk_size; i++) {
//         for (int j = 0; j < chunk_size; j++) {

//             float global_x = offset_x + i;
//             float global_z = offset_z + j;

//             float height = (altitude * v.GetHeight(
//                 global_x / scale, 
//                 global_z / scale, 
//                 0.5f, 
//                 2.0f, 
//                 numoctaves
//             ));

//             int index = i * chunk_size + j;

//             positions[index].postion.x = global_x;
//             positions[index].postion.y = height;
//             positions[index].postion.z = global_z;
//         }
//     }

//     int indices_count = chunk_size - 1;
//     std::vector<unsigned int> indices(indices_count * indices_count * 6);
//     int idx = 0;

//     for (int z = 0; z < indices_count; z++) {
//         for (int x = 0; x < indices_count; x++) {

//             int topLeft = z * chunk_size + x;
//             int topRight = z * chunk_size + x + 1;
//             int bottomLeft = (z + 1) * chunk_size + x;
//             int bottomRight = (z + 1) * chunk_size + x + 1;

//             indices[idx++] = topLeft;
//             indices[idx++] = bottomLeft;
//             indices[idx++] = topRight;

//             indices[idx++] = topRight;
//             indices[idx++] = bottomLeft;
//             indices[idx++] = bottomRight;
//         }
//     }

//     return Mesh(positions, indices, {});
// }

// Mesh TerrainGenerator::generate_value_noise_mesh(
//     std::vector<BiomeSeed> seeds,
//     int camera_x,
//     int camera_z,
//     unsigned int chunk_size, 
//     int offset_x, 
//     int offset_z
// ) {
//     //float scale = 256.0f;

//     std::vector<Vertex> positions(chunk_size*chunk_size);
//     std::vector<float> heights(chunk_size*chunk_size);

//     for (int i = 0; i < chunk_size; i++) {
//         for (int j = 0; j < chunk_size; j++) {

//             float global_x = offset_x + i;
//             float global_z = offset_z + j;

//             // BIOMES current_biome = get_biome(global_x, global_z, seeds);
//             // Biome biome = biome_values[current_biome];
//             // BlendedBiome biome = get_blended_biome(global_x, global_z, seeds);


//             // float height = (biome.altitude * v.GetHeight(
//             //     global_x / biome.scale, 
//             //     global_z / biome.scale, 
//             //     0.5f, 
//             //     2.0f,
//             //     biome.numoctaves
//             // ));

//             //TODO poprawic biomy
//             BiomeBlend blend = get_biome_blend(global_x, global_z, seeds);
//             Biome b1 = biome_values[blend.nearest->biome];
//             Biome b2 = biome_values[blend.second->biome];

//             float h1 = b1.altitude * v.GetHeight(
//                 global_x / b1.scale,
//                 global_z / b1.scale,
//                 0.5f, 2.0f, b1.numoctaves
//             );
//             float h2 = b2.altitude * v.GetHeight(
//                 global_x / b2.scale,
//                 global_z / b2.scale,
//                 0.5f, 2.0f, b2.numoctaves
//             );

//             float height = h1 * blend.w1 + h2 * blend.w2;

//             int index = i * chunk_size + j;

//             positions[index].postion.x = global_x;
//             positions[index].postion.y = height;
//             positions[index].postion.z = global_z;
//         }
//     }
// }