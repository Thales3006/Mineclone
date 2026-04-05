#include "render/mesh/mesh.h"

#include "render/renderLib.h"

template <typename T>
Mesh<T>::Mesh(std::vector<T> vertices, std::vector<unsigned int> indices,
              std::vector<std::shared_ptr<Texture>> textures,
              std::shared_ptr<Shader> shader)
    : VAO(0), VBO(0), EBO(0), vertices(std::move(vertices)),
      indices(std::move(indices)), textures(textures), shader(shader) {
    setupMesh();
}

template <typename T> void Mesh<T>::setupMesh() {
    if (vertices.empty())
        return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    setupAttributes();

    // zerando
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

template <typename T> void Mesh<T>::render() {
    shader->use();
    textures[0]->bind();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                          (void *)offsetof(TerrainVertex, ID));
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
