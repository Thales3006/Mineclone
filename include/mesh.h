#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

        ~Mesh();

        void draw(Shader &shader);

        void setupMesh();
        void clearBuffers();

        private:
            unsigned int VAO, VBO, EBO;

            void resetMesh();
};  

#endif