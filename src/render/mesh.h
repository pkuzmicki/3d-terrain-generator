#pragma once

#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader.h"

struct Vertex {
    glm::vec3 postion;
    glm::vec3 normal;
    glm::vec2 txt_coords;

    float biome1;
    float biome2;
    float blend_alpha;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader &shader);
    
private:
    unsigned int VAO, VBO, EBO;
    void setup_mesh();
};

#endif