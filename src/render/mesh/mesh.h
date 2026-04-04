#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "render/shader.h"
#include "render/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
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
    std::vector<std::shared_ptr<Texture>> textures;

    std::shared_ptr<Shader> shader;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<std::shared_ptr<Texture>> textures,
         std::shared_ptr<Shader> shader);
    Mesh(std::vector<Vertex> vertices,
         std::vector<std::shared_ptr<Texture>> textures,
         std::shared_ptr<Shader> shader);

    ~Mesh();

    virtual void render(int chunkx, int chunkz);

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

  protected:
    void draw();

  private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
