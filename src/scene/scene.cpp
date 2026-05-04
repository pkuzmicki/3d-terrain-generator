#include "scene.h"

Mesh make_cube();

void Scene::update_scene() {}

void Scene::add_chunk(std::pair<int, int> coords) {}

TerrainScene::TerrainScene(AppPanel* ap) {
    this->ap = ap;
    this->minimap = ap->minimap;
    meshes.push_back(make_cube());
}

void TerrainScene::update_scene() {
    int current_chunk_x = std::floor(ap->renderer->get_cam()->position.x / settings.chunk_size);
    int current_chunk_z = std::floor(ap->renderer->get_cam()->position.z / settings.chunk_size);

    std::pair<int, int> coords(current_chunk_x, current_chunk_z);

    ap->renderer->get_cam()->chunk.x = current_chunk_x;
    ap->renderer->get_cam()->chunk.y = current_chunk_z;

    int render_distance = settings.render_distance;

    for (int i = coords.first-render_distance; i <= coords.first+render_distance; i++) {
        for (int j = coords.second-render_distance; j <= coords.second+render_distance; j++) {
            auto iterator = generator.active_chunks.find(std::make_pair(i, j));
            if (iterator == generator.active_chunks.end()) {
                add_chunk(std::make_pair(i, j)); 
            }
        } 
    }
}

void TerrainScene::add_chunk(std::pair<int, int> coords) {    
    float world_x = (int)(settings.chunk_size-1) * coords.first;
    float world_z = (int)(settings.chunk_size-1) * coords.second;

    Mesh new_chunk_mesh = generator.generate_value_noise_mesh( 
        settings.chunk_size, world_x, world_z
    );

    std::cout<<world_x<<" "<<coords.first<<" "<<settings.chunk_size<<"\n";

    generator.active_chunks.emplace(coords, new_chunk_mesh);
    meshes.push_back(new_chunk_mesh);

    minimap->update_map(coords, new_chunk_mesh, settings.chunk_size);
}

Mesh make_cube() {
    std::vector<Vertex> vertices = {
        // FRONT face
        { { -0.5f, -0.5f,  0.5f }, {0.f, 0.f, 1.f}, {0.f, 0.f} },
        { {  0.5f, -0.5f,  0.5f }, {0.f, 0.f, 1.f}, {1.f, 0.f} },
        { {  0.5f,  0.5f,  0.5f }, {0.f, 0.f, 1.f}, {1.f, 1.f} },
        { { -0.5f,  0.5f,  0.5f }, {0.f, 0.f, 1.f}, {0.f, 1.f} },

        // BACK face
        { { -0.5f, -0.5f, -0.5f }, {0.f, 0.f,-1.f}, {1.f, 0.f} },
        { {  0.5f, -0.5f, -0.5f }, {0.f, 0.f,-1.f}, {0.f, 0.f} },
        { {  0.5f,  0.5f, -0.5f }, {0.f, 0.f,-1.f}, {0.f, 1.f} },
        { { -0.5f,  0.5f, -0.5f }, {0.f, 0.f,-1.f}, {1.f, 1.f} },

        // LEFT face
        { { -0.5f, -0.5f, -0.5f }, {-1.f, 0.f, 0.f}, {0.f, 0.f} },
        { { -0.5f, -0.5f,  0.5f }, {-1.f, 0.f, 0.f}, {1.f, 0.f} },
        { { -0.5f,  0.5f,  0.5f }, {-1.f, 0.f, 0.f}, {1.f, 1.f} },
        { { -0.5f,  0.5f, -0.5f }, {-1.f, 0.f, 0.f}, {0.f, 1.f} },

        // RIGHT face
        { { 0.5f, -0.5f, -0.5f }, {1.f, 0.f, 0.f}, {1.f, 0.f} },
        { { 0.5f, -0.5f,  0.5f }, {1.f, 0.f, 0.f}, {0.f, 0.f} },
        { { 0.5f,  0.5f,  0.5f }, {1.f, 0.f, 0.f}, {0.f, 1.f} },
        { { 0.5f,  0.5f, -0.5f }, {1.f, 0.f, 0.f}, {1.f, 1.f} },

        // TOP face
        { { -0.5f,  0.5f,  0.5f }, {0.f, 1.f, 0.f}, {0.f, 0.f} },
        { {  0.5f,  0.5f,  0.5f }, {0.f, 1.f, 0.f}, {1.f, 0.f} },
        { {  0.5f,  0.5f, -0.5f }, {0.f, 1.f, 0.f}, {1.f, 1.f} },
        { { -0.5f,  0.5f, -0.5f }, {0.f, 1.f, 0.f}, {0.f, 1.f} },

        // BOTTOM face
        { { -0.5f, -0.5f,  0.5f }, {0.f,-1.f, 0.f}, {0.f, 1.f} },
        { {  0.5f, -0.5f,  0.5f }, {0.f,-1.f, 0.f}, {1.f, 1.f} },
        { {  0.5f, -0.5f, -0.5f }, {0.f,-1.f, 0.f}, {1.f, 0.f} },
        { { -0.5f, -0.5f, -0.5f }, {0.f,-1.f, 0.f}, {0.f, 0.f} },
    };

    std::vector<unsigned int> indices = {
        // FRONT
        0, 1, 2,   2, 3, 0,
        // BACK
        4, 5, 6,   6, 7, 4,
        // LEFT
        8, 9,10,  10,11, 8,
        // RIGHT
       12,13,14,  14,15,12,
        // TOP
       16,17,18,  18,19,16,
        // BOTTOM
       20,21,22,  22,23,20
    };

    return Mesh(vertices, indices, {});
}