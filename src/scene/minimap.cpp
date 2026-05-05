#include "minimap.h"

#include <iostream>

#include "core/app_panel.h"

MiniMap::MiniMap(){}

std::unordered_map<std::pair<int, int>, unsigned int, PairHash>& MiniMap::get_minimap() {
    return minimaps_of_all_chunks;
}

void MiniMap::update_map(std::pair<int, int> chunk_coords, Mesh chunk_mesh, int map_size, AppPanel* ap) {
    unsigned char* new_map_texture = new unsigned char [map_size*map_size*3];

    int idx = 0;
    for (int i = 0; i < map_size*map_size*3; i+=3) {

        unsigned char r, g, b;
        glm::vec3 v = chunk_mesh.vertices[idx++].postion;
        int x = v.x;
        int y = v.y;
        int z = v.z;

        if (ap->s_manager->current_scene->get_generator_settings()->mode == MAP_MODE::BIOME) {

            float t = ap->s_manager->current_scene->get_generator()->get_point_height(x, z, 
                ap->s_manager->current_scene->get_generator_settings()->temp_alt,
                ap->s_manager->current_scene->get_generator()->get_temp_scale(),
                ap->s_manager->current_scene->get_generator()->get_temp_offset()
            );
            float h = ap->s_manager->current_scene->get_generator()->get_point_height(x, z, 
                ap->s_manager->current_scene->get_generator_settings()->height_alt,
                ap->s_manager->current_scene->get_generator()->get_h_scale(),
                ap->s_manager->current_scene->get_generator()->get_h_offset()
            );

            BIOMES biome = ap->s_manager->current_scene->get_generator()->define_biome(h, t);

            switch (biome) {
                case BIOMES::PLAINS: {r=93;  g=177; b=87;}  break;
                case BIOMES::MOUNTAIS: r=120; g=100; b=80;  break;
                case BIOMES::WATER: r=9;   g=107; b=173; break;
                case BIOMES::DESERT: r=237; g=201; b=100; break;
                case BIOMES::MESA: r=200; g=100; b=50;  break;
                case BIOMES::WARM_WATER: r=0;   g=150; b=200; break;
                case BIOMES::SNOW: r=220; g=230; b=240; break;
                case BIOMES::ICEBEARG: r=180; g=210; b=230; break;
                case BIOMES::COLD_WATER: r=20;  g=60;  b=120; break;
            }

        } else if (ap->s_manager->current_scene->get_generator_settings()->mode == MAP_MODE::HEIGHT) {

            if (y < 10) {r = 0; g = 83; b = 147;}
            else if (y >= 10 && y < 20) {r = 138; g = 203; b = 223;}
            else if (y >= 20 && y < 40) {r = 52; g = 135; b = 48;}
            else if (y >= 40 && y < 60) {r = 93; g = 177; b = 87;}
            else if (y >= 60 && y < 75) {r = 244; g = 206; b = 82;}
            else {r = 178; g = 93; b = 42;};

        } else {
            float t = ap->s_manager->current_scene->get_generator()->get_point_height(x, z, 
                ap->s_manager->current_scene->get_generator_settings()->temp_alt, 
                ap->s_manager->current_scene->get_generator()->get_temp_scale(),
                ap->s_manager->current_scene->get_generator()->get_temp_offset()
            );

            // r = (unsigned char)(t * 255);
            // g = (unsigned char)(t < 0.5f ? t * 2 * 255 : (1.0f - t) * 2 * 255);
            // b = (unsigned char)((1.0f - t) * 255);

            if (t < 0.33f) {r=0; g=150; b=255;}
            else if (t < 0.66f) {r=0; g=200; b=0;}
            else {r=255; g=100; b=0;}
        }

        new_map_texture[i] = r;
        new_map_texture[i+1] = g;
        new_map_texture[i+2] = b;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map_size, map_size, 0, GL_RGB, GL_UNSIGNED_BYTE, new_map_texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    delete[] new_map_texture;

    minimaps_of_all_chunks.emplace(chunk_coords, texture);
}