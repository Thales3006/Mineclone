#include "render/mesh/mesh.h"

#include "render/renderLib.h"

template <typename T>
Mesh<T>::Mesh(Geometry<T> geometry, std::shared_ptr<Texture> texture,
              std::shared_ptr<Shader> shader)
    : VAO(0), VBO(0), EBO(0), geometry(geometry), texture(texture),
      shader(shader) {}

template <typename T> void Mesh<T>::setupMesh() {
    if (VAO != 0) {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

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
}

template <typename T> void Mesh<T>::render() {
    shader->use();
    texture->bind();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, geometry.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

template <typename T> Mesh<T>::~Mesh() {
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
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
