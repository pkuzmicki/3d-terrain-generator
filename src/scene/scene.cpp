#include "scene.h"

static AppPanel* ap = &AppPanel::getInstance();

Mesh make_cube();

void Scene::update_scene() {}

void Scene::add_chunk(std::pair<int, int> coords) {}

TerrainScene::TerrainScene() {
    meshes.push_back(make_cube());
}

void TerrainScene::update_scene() {
    int current_chunk_x = std::floor(ap->renderer->main_camera->position.x / settings.chunk_size);
    int current_chunk_z = std::floor(ap->renderer->main_camera->position.z / settings.chunk_size);

    std::pair<int, int> coords(current_chunk_x, current_chunk_z);

    ap->renderer->main_camera->chunk.x = current_chunk_x;
    ap->renderer->main_camera->chunk.y = current_chunk_z;

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
    auto biome_seeds = generate_biome_seeds(coords.first, coords.second, settings.chunk_size, generator.seed);

    Mesh new_chunk_mesh = generator.generate_value_noise_mesh(
        biome_seeds,
        ap->renderer->main_camera->position.x,
        ap->renderer->main_camera->position.z,
        settings.chunk_size,
        (settings.chunk_size-1) * coords.first, 
        (settings.chunk_size-1) * coords.second
    );
    
    generator.active_chunks.emplace(coords, new_chunk_mesh);
    meshes.push_back(new_chunk_mesh);
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