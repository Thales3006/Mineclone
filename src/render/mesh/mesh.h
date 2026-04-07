#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "render/mesh/geometry.h"
#include "render/shader.h"
#include "render/texture.h"

#include <memory>

template <typename T> class Mesh {
  public:
    Geometry<T> geometry;
    std::shared_ptr<Texture> texture;

    std::shared_ptr<Shader> shader;

    Mesh(Geometry<T> geometry, std::shared_ptr<Texture> texture,
         std::shared_ptr<Shader> shader);

    ~Mesh();

    void render();
    void setupMesh();

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

  private:
    unsigned int VAO, VBO, EBO;
    void setupAttributes();
};

#endif
