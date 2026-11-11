#include "render/mesh/geometry.h"

template <> void Geometry<TerrainVertex>::addVertexOffset(glm::vec3 offset) {
    for (auto &vertex : vertices) {
        vertex.position += offset;
    }
}
