#ifndef BLOCK_REGISTRY_H
#define BLOCK_REGISTRY_H

#include "world/terrain/block/block.h"

#include <glm/glm.hpp>
#include <vector>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

enum class BlockModel { Cube, Plant };

struct BlockDef {
    BlockModel model;
    BlockTextureID faces[6]; // front, back, left, right, up, down
    glm::vec3 tint[6];
    std::vector<AABB> selection;
    std::vector<AABB> collision;
    bool occludes;
};

const BlockDef &blockDef(BlockID id);

#endif
