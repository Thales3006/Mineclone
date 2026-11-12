#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "render/mesh/geometry.h"
#include "render/shader.h"
#include "render/texture.h"

#include <memory>

template <typename T> class Mesh {
  public:
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader> shader;

    Mesh(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader);
    Mesh(Geometry<T> geometry, std::shared_ptr<Texture> texture,
         std::shared_ptr<Shader> shader);

    Mesh(const Mesh &other);
    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(const Mesh &other);
    Mesh &operator=(Mesh &&other) noexcept;
    ~Mesh();

    void render();

    const Geometry<T> &getGeometry() const;
    void setGeometry(Geometry<T> geometry);
    void setDrawMode(unsigned int mode);
    unsigned int addVertex(const T &vertex);
    void addTriangle(unsigned int a, unsigned int b, unsigned int c);
    void append(const Geometry<T> &other);
    void removeVertices(unsigned int start, unsigned int count);
    void clear();

  private:
    Geometry<T> geometry;
    unsigned int VAO, VBO, EBO;
    unsigned int drawMode;
    bool dirty;

    void upload();
    void release();
    void setupAttributes();
};

#endif
