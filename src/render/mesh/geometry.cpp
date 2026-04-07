#include "render/mesh/geometry.h"

template <typename T>
Geometry<T>::Geometry(std::vector<T> vertices,
                      std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {}

template <typename T> void Geometry<T>::append(const Geometry<T> &other) {
    unsigned int offset = vertices.size();
    vertices.insert(vertices.end(), other.vertices.begin(),
                    other.vertices.end());
    for (auto i : other.indices)
        indices.push_back(i + offset);
}

template <> void Geometry<TerrainVertex>::addVertexOffset(glm::vec3 offset) {
    for (auto &vertex : vertices) {
        vertex.position += offset;
    }
}

template class Geometry<TerrainVertex>;
