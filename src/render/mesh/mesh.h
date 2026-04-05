#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "render/shader.h"
#include "render/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

struct TerrainVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    float ID;
};

struct UIVertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

template <typename T> class Mesh {
  public:
    std::vector<T> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    std::shared_ptr<Shader> shader;

    Mesh(std::vector<T> vertices, std::vector<unsigned int> indices,
         std::vector<std::shared_ptr<Texture>> textures,
         std::shared_ptr<Shader> shader);
    Mesh(std::vector<T> vertices,
         std::vector<std::shared_ptr<Texture>> textures,
         std::shared_ptr<Shader> shader);

    ~Mesh();

    void render();

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

  private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    void setupAttributes();
};

#endif
