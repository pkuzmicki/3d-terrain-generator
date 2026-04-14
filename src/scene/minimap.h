#pragma once

#ifndef MINIMAP_H
#define MINIMAP_H

#include <GLFW/glfw3.h>

#include "render/mesh.h"
#include "core/pairhash.h"

class MiniMap {
private:
    std::unordered_map<std::pair<int, int>, unsigned int, PairHash> minimaps_of_all_chunks;
public:
    MiniMap();
    const std::unordered_map<std::pair<int, int>, unsigned int, PairHash>& get_minimap();

    void update_map(std::pair<int, int> chunk_coords, Mesh chunk_mesh, int map_size);
    //void draw_map(unsigned int txt, int pixels_per_chunk, ImVec2 offset, std::pair<int, int> coords);
};

#endif 