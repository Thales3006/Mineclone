#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    float ID;
};

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    ~Mesh();

    void draw(Shader &shader);

    void setupMesh();
    bool isSetup = false; // temporary fix for big threading problem

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

  private:
    unsigned int VAO, VBO, EBO;
};

#endif
