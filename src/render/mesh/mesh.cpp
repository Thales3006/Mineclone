#include "render/mesh/mesh.h"

#include "render/renderLib.h"

#include <utility>

template <typename T>
Mesh<T>::Mesh(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
    : texture(std::move(texture)), shader(std::move(shader)), geometry(),
      VAO(0), VBO(0), EBO(0), dirty(true) {}

template <typename T>
Mesh<T>::Mesh(Geometry<T> geometry, std::shared_ptr<Texture> texture,
              std::shared_ptr<Shader> shader)
    : texture(std::move(texture)), shader(std::move(shader)),
      geometry(std::move(geometry)), VAO(0), VBO(0), EBO(0), dirty(true) {}

template <typename T>
Mesh<T>::Mesh(const Mesh &other)
    : texture(other.texture), shader(other.shader), geometry(other.geometry),
      VAO(0), VBO(0), EBO(0), dirty(true) {}

template <typename T>
Mesh<T>::Mesh(Mesh &&other) noexcept
    : texture(std::move(other.texture)), shader(std::move(other.shader)),
      geometry(std::move(other.geometry)), VAO(other.VAO), VBO(other.VBO),
      EBO(other.EBO), dirty(other.dirty) {
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.dirty = true;
}

template <typename T> Mesh<T> &Mesh<T>::operator=(const Mesh &other) {
    if (this != &other) {
        release();
        texture = other.texture;
        shader = other.shader;
        geometry = other.geometry;
        dirty = true;
    }
    return *this;
}

template <typename T> Mesh<T> &Mesh<T>::operator=(Mesh &&other) noexcept {
    if (this != &other) {
        release();
        texture = std::move(other.texture);
        shader = std::move(other.shader);
        geometry = std::move(other.geometry);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        dirty = other.dirty;
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.dirty = true;
    }
    return *this;
}

template <typename T> Mesh<T>::~Mesh() { release(); }

template <typename T> void Mesh<T>::release() {
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
    VAO = VBO = EBO = 0;
}

template <typename T> void Mesh<T>::upload() {
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(T),
                 geometry.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 geometry.indices.size() * sizeof(unsigned int),
                 geometry.indices.data(), GL_STATIC_DRAW);

    setupAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    dirty = false;
}

template <typename T> void Mesh<T>::render() {
    if (dirty)
        upload();

    shader->use();
    texture->bind();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, geometry.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

template <typename T> const Geometry<T> &Mesh<T>::getGeometry() const {
    return geometry;
}

template <typename T> void Mesh<T>::setGeometry(Geometry<T> newGeometry) {
    geometry = std::move(newGeometry);
    dirty = true;
}

template <typename T> unsigned int Mesh<T>::addVertex(const T &vertex) {
    dirty = true;
    return geometry.addVertex(vertex);
}

template <typename T>
void Mesh<T>::addTriangle(unsigned int a, unsigned int b, unsigned int c) {
    geometry.addTriangle(a, b, c);
    dirty = true;
}

template <typename T> void Mesh<T>::append(const Geometry<T> &other) {
    geometry.append(other);
    dirty = true;
}

template <typename T>
void Mesh<T>::removeVertices(unsigned int start, unsigned int count) {
    geometry.removeVertices(start, count);
    dirty = true;
}

template <typename T> void Mesh<T>::clear() {
    geometry.clear();
    dirty = true;
}

template <> void Mesh<TerrainVertex>::setupAttributes() {

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, uv));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, color));
}

template <> void Mesh<UIVertex>::setupAttributes() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex),
                          (void *)offsetof(UIVertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex),
                          (void *)offsetof(UIVertex, uv));
}

template class Mesh<TerrainVertex>;
template class Mesh<UIVertex>;
