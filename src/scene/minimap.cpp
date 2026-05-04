#include "minimap.h"

#include <iostream>

MiniMap::MiniMap(){}

const std::unordered_map<std::pair<int, int>, unsigned int, PairHash>& MiniMap::get_minimap() {
    return minimaps_of_all_chunks;
}

void MiniMap::update_map(std::pair<int, int> chunk_coords, Mesh chunk_mesh, int map_size) {
    unsigned char* new_map_texture = new unsigned char [map_size*map_size*3];

    int idx = 0;
    for (int i = 0; i < map_size*map_size*3; i+=3) {

        unsigned char r, g, b;
        int y = chunk_mesh.vertices[idx++].postion.y;
        // if (y < 20) r = 255, g = 0, b = 0;
        // else if (y > 20 && y < 100) r = 0, g = 255, b = 0;
        // else r = 0, g = 0, b = 255;
        if (y < 30) {r = 9; g = 107; b = 173;}
        else if (y > 30 && y < 80) {r = 93; g = 177; b = 87;}
        else {r = 178; g = 93; b = 42;};
        
        new_map_texture[i] = r;
        new_map_texture[i+1] = g;
        new_map_texture[i+2] = b;
        //std::cout<<"txt: "<<new_map_texture[i]<<"\n";
    }

    // unsigned char r = 255, g = 255, b = 255;
    // for(BiomeSeed s : seeds) {
    //     if (s.x > chunk_coords.first*map_size && s.x < chunk_coords.first*map_size + map_size) {
    //         if (s.z > chunk_coords.second*map_size && s.z < chunk_coords.second*map_size + map_size) {

    //             new_map_texture[i] = r;
    //             new_map_texture[i+1] = g;
    //             new_map_texture[i+2] = b;
    //         }
    //     }
    // }

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
    //std::cout<<"a"<<minimaps_of_all_chunks.at(chunk_coords);
}

// void MiniMap::draw_map(unsigned int txt, int pixels_per_chunk, ImVec2 offset, std::pair<int, int> coords) {
//     ImGui::Begin("Map", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

//     ImVec2 pos = ImGui::GetCursorScreenPos();
//     ImVec2 map_pos(pos.x + offset.x + pixels_per_chunk * coords.first, pos.y + offset.y + pixels_per_chunk * coords.second);
//     ImGui::GetWindowDrawList()->AddImage((ImTextureID)txt, map_pos, ImVec2(map_pos.x + pixels_per_chunk, map_pos.y + pixels_per_chunk), ImVec2(0, 1), ImVec2(1, 0));

//     ImGui::End();
// }