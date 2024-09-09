#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include "textureClass.h"
#include "shaderClass.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  

#endif