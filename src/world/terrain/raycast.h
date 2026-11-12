#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>
#include <optional>

class ChunkManager;

struct RaycastHit {
    glm::ivec3 block;
    glm::ivec3 normal;
};

std::optional<RaycastHit> raycastVoxel(ChunkManager &chunkManager, int chunkx,
                                       int chunkz, glm::vec3 origin,
                                       glm::vec3 direction, float maxDistance);

#endif
