#include "terrain.h"
#include "noise/valuenoise.h"
#include "render/renderer.h"

#include <vector>
#include <chrono>
#include <algorithm>

ValueNoiseGeneration TerrainGenerator::v_biomes[10];

TerrainGenerator::TerrainGenerator() {
    init_value_noise();
}

float x;
float z;

void TerrainGenerator::init_value_noise() {
    seed = std::time(0);
    std::srand(seed);
    std::cout<<"seed: "<<seed<<"\n";

    x = (float)rand();
    z = (float)rand();

    v_biomes[BIOMES::COUNT].Initialize();
    v_biomes[BIOMES::COUNT].SetValueTable(everest_distribution, everest_points);  

    v_biomes[BIOMES::PLAINS].Initialize();
    v_biomes[BIOMES::PLAINS].SetValueTable(stepy_distribution, stepy_points);  

    v_biomes[BIOMES::MOUNTAIS].Initialize();
    v_biomes[BIOMES::MOUNTAIS].SetValueTable(everest_distribution, everest_points);    

    v_biomes[BIOMES::WATER].Initialize();
    v_biomes[BIOMES::WATER].SetValueTable(baltyk_distribution, baltyk_points);

    v_biomes[BIOMES::SNOW].Initialize();
    v_biomes[BIOMES::SNOW].SetValueTable(snow_distribution, snow_points);

    v_biomes[BIOMES::ICEBEARG].Initialize();
    v_biomes[BIOMES::ICEBEARG].SetValueTable(fiordy_distribution, fiordy_points);

    v_biomes[BIOMES::COLD_WATER].Initialize();
    v_biomes[BIOMES::COLD_WATER].SetValueTable(norweskie_distribution, norweskie_points);

    v_biomes[BIOMES::DESERT].Initialize();
    v_biomes[BIOMES::DESERT].SetValueTable(sahara_distribution, sahara_points);

    v_biomes[BIOMES::MESA].Initialize();
    v_biomes[BIOMES::MESA].SetValueTable(canyon_distribution, canyon_points);

    v_biomes[BIOMES::WARM_WATER].Initialize();
    v_biomes[BIOMES::WARM_WATER].SetValueTable(srodziemne_distribution, srodziemne_points);
}

Mesh TerrainGenerator::generate_value_noise_mesh(unsigned int chunk_size, int offset_x, int offset_z) {
    std::vector<Vertex> positions(chunk_size*chunk_size);

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {

            float global_x = offset_x + i;
            float global_z = offset_z + j;

            float t = get_point_height(global_x, global_z, t_alt, TEMP_SCALE, TEMP_OFFSET);
            float h = get_point_height(global_x, global_z, h_alt, H_SCALE, H_OFFSET);

            BIOMES b1, b2;
            float alpha = get_blend(h, t, b1, b2);

            float h1 = get_point_height_for_biome(global_x, global_z, b1);
            float h2 = get_point_height_for_biome(global_x, global_z, b2);
          
            
            float height = h1 * (1.0f - alpha) + h2 * alpha;

            int index = i * chunk_size + j;
            positions[index].postion.x = global_x;
            positions[index].postion.y = height;
            positions[index].postion.z = global_z;

            positions[index].biome1 = (float)b1;
            positions[index].biome2 = (float)b2;
            positions[index].blend_alpha = alpha;
        }
    }

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {
            int idx = i * chunk_size + j;

            int left  = std::clamp(i - 1, 0, (int)chunk_size - 1) * chunk_size + j;
            int right = std::clamp(i + 1, 0, (int)chunk_size - 1) * chunk_size + j;
            int down  = i * chunk_size + std::clamp(j - 1, 0, (int)chunk_size - 1);
            int up    = i * chunk_size + std::clamp(j + 1, 0, (int)chunk_size - 1);

            glm::vec3 dx = positions[right].postion - positions[left].postion;
            glm::vec3 dz = positions[up].postion    - positions[down].postion;

            positions[idx].normal = glm::normalize(glm::cross(dz, dx));
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

Mesh TerrainGenerator::generate_value_noise_single_biome_mesh(unsigned int chunk_size, int offset_x, int offset_z, unsigned int biome_index) {
    std::vector<Vertex> positions(chunk_size*chunk_size);
    std::vector<float> heights(chunk_size*chunk_size);

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {

            float global_x = offset_x + i;
            float global_z = offset_z + j;

            BIOMES b = (biome_index < 10) ? (BIOMES)biome_index : BIOMES::PLAINS;

            float height = get_point_height_for_biome(global_x, global_z, b);

            int index = i * chunk_size + j;

            positions[index].postion.x = global_x;
            positions[index].postion.y = height;
            positions[index].postion.z = global_z;

            positions[index].biome1 = (float)b;
            positions[index].biome2 = (float)b;
        }
    }

    for (int i = 0; i < chunk_size; i++) {
        for (int j = 0; j < chunk_size; j++) {
            int idx = i * chunk_size + j;

            int left  = std::clamp(i - 1, 0, (int)chunk_size - 1) * chunk_size + j;
            int right = std::clamp(i + 1, 0, (int)chunk_size - 1) * chunk_size + j;
            int down  = i * chunk_size + std::clamp(j - 1, 0, (int)chunk_size - 1);
            int up    = i * chunk_size + std::clamp(j + 1, 0, (int)chunk_size - 1);

            glm::vec3 dx = positions[right].postion - positions[left].postion;
            glm::vec3 dz = positions[up].postion    - positions[down].postion;

            positions[idx].normal = glm::normalize(glm::cross(dz, dx));
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

float TerrainGenerator::get_point_height(float x, float z, unsigned int numoctaves, float scale, float offset) {
    float global_x = x / scale + offset;
    float global_z = z / scale + offset;
    return v_biomes[BIOMES::COUNT].GetHeight(global_x, global_z, 0.5f, 2.0f, numoctaves);
}

float TerrainGenerator::get_point_height_for_biome(float x, float z, BIOMES b) {
    float global_x = x / TERRAIN_SCALE;
    float global_z = z / TERRAIN_SCALE;
    float height = v_biomes[b].GetHeight(global_x, global_z, 0.5f, 2.0f, biome_values[b].numoctaves);
    return biome_values[b].altitude * height;
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

float TerrainGenerator::smoothstep(float edge0, float edge1, float x) {
    float val = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return val * val * (3.0f - 2.0f * val);
}

float TerrainGenerator::get_blend(float h, float t, BIOMES& b1, BIOMES& b2) {
    b1 = define_biome(h, t);
    b2 = b1;

    float wh = smoothstep(0.0f, 1.0f, std::abs(h - H_WATER) / BLEND_RANGE);
    float wm = smoothstep(0.0f, 1.0f, std::abs(h - H_MOUNT) / BLEND_RANGE);
    float wc = smoothstep(0.0f, 1.0f, std::abs(t - T_COLD)  / BLEND_RANGE);
    float ww = smoothstep(0.0f, 1.0f, std::abs(t - T_WARM)  / BLEND_RANGE);

    float w = std::min({wh, wm, wc, ww});

    if (w >= 1.0f) return 0.0f;

    float alpha = 1.0f - w;
    float border_offset = 0.001f;

    if (wh <= wm && wh <= wc && wh <= ww) {
        b1 = define_biome(H_WATER - border_offset, t);
        b2 = define_biome(H_WATER + border_offset, t);
        alpha = smoothstep(H_WATER - BLEND_RANGE, H_WATER + BLEND_RANGE, h);
    } else if (wm <= wc && wm <= ww) {
        b1 = define_biome(H_MOUNT - border_offset, t);
        b2 = define_biome(H_MOUNT + border_offset, t);
        alpha = smoothstep(H_MOUNT - BLEND_RANGE, H_MOUNT + BLEND_RANGE, h);
    } else if (wc <= ww) {
        b1 = define_biome(h, T_COLD - border_offset);
        b2 = define_biome(h, T_COLD + border_offset);
        alpha = smoothstep(T_COLD - BLEND_RANGE, T_COLD + BLEND_RANGE, t);
    } else {
        b1 = define_biome(h, T_WARM - border_offset);
        b2 = define_biome(h, T_WARM + border_offset);
        alpha = smoothstep(T_WARM - BLEND_RANGE, T_WARM + BLEND_RANGE, t);
    }

    return alpha;
}