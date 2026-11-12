#ifndef RAYCAST_H
#define RAYCAST_H

#include "world/terrain/block/block.h"

#include <glm/glm.hpp>
#include <optional>

class ChunkManager;

struct RaycastHit {
    glm::ivec3 block;
    glm::ivec3 normal;
    BlockID id;
};

std::optional<RaycastHit> raycastVoxel(ChunkManager &chunkManager,
                                       glm::ivec2 chunk, glm::vec3 origin,
                                       glm::vec3 direction, float maxDistance);

#endif
