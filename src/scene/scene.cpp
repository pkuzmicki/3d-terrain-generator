#include "scene.h"

#include <unordered_set>
#include <utility>
#include <chrono>

Mesh make_cube();

void Scene::update_scene() {}

void Scene::add_chunk(std::pair<int, int> coords) {}

TerrainScene::TerrainScene(AppPanel* ap) {
    this->ap = ap;
    this->minimap = ap->minimap;

    settings.world_seed = std::time(0);
    std::srand(settings.world_seed);
    std::cout<<"seed: "<<settings.world_seed<<"\n";

    generator.seed = settings.world_seed;
}

void TerrainScene::update_scene() {
    generator.set_altitudes(settings.temp_alt, settings.height_alt);

    int current_chunk_x = std::floor(ap->renderer->get_cam()->position.x / settings.chunk_size);
    int current_chunk_z = std::floor(ap->renderer->get_cam()->position.z / settings.chunk_size);

    std::pair<int, int> coords(current_chunk_x, current_chunk_z);

    ap->renderer->get_cam()->chunk.x = current_chunk_x;
    ap->renderer->get_cam()->chunk.y = current_chunk_z;

    int render_distance = settings.render_distance;

    std::unordered_set<std::pair<int,int>, PairHash> new_chunks;

    for (int i = coords.first-render_distance; i <= coords.first+render_distance; i++) {
        for (int j = coords.second-render_distance; j <= coords.second+render_distance; j++) {
            new_chunks.emplace(i, j);

            if (generator.active_chunks.find({i, j}) == generator.active_chunks.end()) {
                add_chunk({i, j});
            }
        } 
    }

    // for (auto iterator = generator.active_chunks.begin(); iterator != generator.active_chunks.end();) {
    //     if (new_chunks.find(iterator->first) == new_chunks.end()) {
    //         iterator = generator.active_chunks.erase(iterator);
    //     } else {
    //         iterator++;
    //     }
    // }
}

void TerrainScene::add_chunk(std::pair<int, int> coords) {    
    float world_x = (int)(settings.chunk_size-1) * coords.first;
    float world_z = (int)(settings.chunk_size-1) * coords.second;

    Mesh new_chunk_mesh = (settings.is_in_single_biome_mode) ? 
        generator.generate_value_noise_single_biome_mesh(settings.chunk_size, world_x, world_z, settings.biome_index): 
        generator.generate_value_noise_mesh(settings.chunk_size, world_x, world_z);

    generator.active_chunks.emplace(coords, new_chunk_mesh);

    minimap->update_map(coords, new_chunk_mesh, settings.chunk_size, ap);
}