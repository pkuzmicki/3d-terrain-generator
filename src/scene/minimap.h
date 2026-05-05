#pragma once

#ifndef MINIMAP_H
#define MINIMAP_H

#include <GLFW/glfw3.h>

#include "render/mesh.h"
#include "core/pairhash.h"

class AppPanel;

enum MAP_MODE {
    BIOME, 
    HEIGHT,
    TEMPERATURE
};

class MiniMap {
private:
    AppPanel* ap;
    std::unordered_map<std::pair<int, int>, unsigned int, PairHash> minimaps_of_all_chunks;
public:
    MiniMap();
    std::unordered_map<std::pair<int, int>, unsigned int, PairHash>& get_minimap();

    void update_map(std::pair<int, int> chunk_coords, Mesh chunk_mesh, int chunk_size, AppPanel* ap);
};

#endif 