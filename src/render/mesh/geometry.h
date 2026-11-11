#ifndef GEOMETRY_CLASS_H
#define GEOMETRY_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <utility>
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

    Geometry() = default;

    Geometry(std::vector<T> vertices, std::vector<unsigned int> indices)
        : vertices(std::move(vertices)), indices(std::move(indices)) {}

    unsigned int addVertex(const T &vertex) {
        vertices.push_back(vertex);
        return static_cast<unsigned int>(vertices.size() - 1);
    }

    void addTriangle(unsigned int a, unsigned int b, unsigned int c) {
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }

    void append(const Geometry<T> &other) {
        unsigned int offset = static_cast<unsigned int>(vertices.size());
        vertices.insert(vertices.end(), other.vertices.begin(),
                        other.vertices.end());
        for (unsigned int index : other.indices)
            indices.push_back(index + offset);
    }

    void removeVertices(unsigned int start, unsigned int count) {
        unsigned int size = static_cast<unsigned int>(vertices.size());
        if (count == 0 || start >= size)
            return;

        unsigned int end = start + count;
        if (end > size) {
            end = size;
            count = end - start;
        }

        std::vector<unsigned int> kept;
        kept.reserve(indices.size());
        for (std::size_t i = 0; i + 3 <= indices.size(); i += 3) {
            unsigned int a = indices[i];
            unsigned int b = indices[i + 1];
            unsigned int c = indices[i + 2];

            bool touchesRemoved = (a >= start && a < end) ||
                                  (b >= start && b < end) ||
                                  (c >= start && c < end);
            if (touchesRemoved)
                continue;

            kept.push_back(a >= end ? a - count : a);
            kept.push_back(b >= end ? b - count : b);
            kept.push_back(c >= end ? c - count : c);
        }

        indices = std::move(kept);
        vertices.erase(vertices.begin() + start, vertices.begin() + end);
    }

    void clear() {
        vertices.clear();
        indices.clear();
    }

    void addVertexOffset(glm::vec3 offset);
};

#endif
