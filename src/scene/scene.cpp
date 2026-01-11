#include "scene.h"

Mesh make_cube();

void Scene::update_scene() {}

void TerrainScene::update_scene() {
    meshes.pop_back();
    meshes.push_back(generator.generate_value_noise_mesh(settings.size, settings.numoctaves, settings.altitude));
}

TerrainScene::TerrainScene() {
    meshes.push_back(make_cube());
    meshes.push_back(generator.generate_value_noise_mesh());
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