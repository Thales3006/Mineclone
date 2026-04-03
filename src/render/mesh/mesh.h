#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "render/shader.h"
#include "render/texture.h"

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

    ~Mesh();

    virtual void render(Shader &shader, int chunkx, int chunkz);

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

  protected:
    void draw(Shader &shader);

  private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
