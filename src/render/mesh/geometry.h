#ifndef GEOMETRY_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct TerrainVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 color;
};

struct UIVertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

template <typename T> struct Geometry {
    std::vector<T> vertices;
    std::vector<unsigned int> indices;

    Geometry<T>() : vertices(), indices() {}

    Geometry<T>(std::vector<T> vertices, std::vector<unsigned int> indices);

    void append(const Geometry<T> &other);

    void addVertexOffset(glm::vec3 offset);
};

#endif
