#include "world/terrain/raycast.h"

#include "world/terrain/block/block.h"
#include "world/terrain/chunk/chunkManager.h"

#include <limits>

std::optional<RaycastHit> raycastVoxel(ChunkManager &chunkManager, int chunkx,
                                       int chunkz, glm::vec3 origin,
                                       glm::vec3 direction, float maxDistance) {
    glm::vec3 dir = glm::normalize(direction);

    glm::ivec3 voxel = glm::floor(origin);
    glm::ivec3 step;
    glm::vec3 tMax;
    glm::vec3 tDelta;

    for (int a = 0; a < 3; a++) {
        if (dir[a] > 0.0f) {
            step[a] = 1;
            tMax[a] = (float(voxel[a] + 1) - origin[a]) / dir[a];
            tDelta[a] = 1.0f / dir[a];
        } else if (dir[a] < 0.0f) {
            step[a] = -1;
            tMax[a] = (origin[a] - float(voxel[a])) / -dir[a];
            tDelta[a] = -1.0f / dir[a];
        } else {
            step[a] = 0;
            tMax[a] = std::numeric_limits<float>::infinity();
            tDelta[a] = std::numeric_limits<float>::infinity();
        }
    }

    glm::ivec3 normal(0);
    float t = 0.0f;

    while (t <= maxDistance) {
        if (voxel.y >= 0 && voxel.y < CHUNK_HEIGHT &&
            chunkManager.getBlock(chunkx, chunkz, glm::vec3(voxel) + 0.5f).id !=
                BlockID::air)
            return RaycastHit{voxel, normal};

        int axis = 0;
        if (tMax.y < tMax[axis])
            axis = 1;
        if (tMax.z < tMax[axis])
            axis = 2;

        voxel[axis] += step[axis];
        t = tMax[axis];
        tMax[axis] += tDelta[axis];

        normal = glm::ivec3(0);
        normal[axis] = -step[axis];
    }

    return std::nullopt;
}
