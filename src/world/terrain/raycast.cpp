#include "world/terrain/raycast.h"

#include "world/terrain/block/block.h"
#include "world/terrain/block/blockRegistry.h"
#include "world/terrain/chunk/chunkManager.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace {

bool rayBox(glm::vec3 origin, glm::vec3 dir, glm::vec3 bmin, glm::vec3 bmax,
            float maxDistance, float &outT, glm::ivec3 &outNormal) {
    float tmin = 0.0f;
    float tmax = maxDistance;
    glm::ivec3 normal(0);

    for (int a = 0; a < 3; a++) {
        if (std::abs(dir[a]) < 1e-8f) {
            if (origin[a] < bmin[a] || origin[a] > bmax[a])
                return false;
            continue;
        }

        float inv = 1.0f / dir[a];
        float tNear = (bmin[a] - origin[a]) * inv;
        float tFar = (bmax[a] - origin[a]) * inv;
        int sign = -1;
        if (tNear > tFar) {
            std::swap(tNear, tFar);
            sign = 1;
        }

        if (tNear > tmin) {
            tmin = tNear;
            normal = glm::ivec3(0);
            normal[a] = sign;
        }
        if (tFar < tmax)
            tmax = tFar;
        if (tmin > tmax)
            return false;
    }

    outT = tmin;
    outNormal = normal;
    return true;
}

} // namespace

std::optional<RaycastHit> raycastVoxel(ChunkManager &chunkManager, int chunkx,
                                       int chunkz, glm::vec3 origin,
                                       glm::vec3 direction, float maxDistance) {
    std::optional<RegionSnapshot> region = chunkManager.snapshotRegion(chunkx, chunkz);
    if (!region)
        return std::nullopt;

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

    float t = 0.0f;

    while (t <= maxDistance) {
        if (voxel.y >= 0 && voxel.y < CHUNK_HEIGHT) {
            BlockID id = region->getBlock(voxel.x, voxel.y, voxel.z).id;
            if (id != BlockID::air) {
                const BlockDef &def = blockDef(id);

                float bestT = maxDistance;
                glm::ivec3 bestNormal(0);
                bool hit = false;
                for (const AABB &box : def.selection) {
                    float hitT;
                    glm::ivec3 hitNormal;
                    if (rayBox(origin, dir, glm::vec3(voxel) + box.min,
                               glm::vec3(voxel) + box.max, maxDistance, hitT,
                               hitNormal) &&
                        hitT <= bestT) {
                        bestT = hitT;
                        bestNormal = hitNormal;
                        hit = true;
                    }
                }

                if (hit)
                    return RaycastHit{voxel, bestNormal, id};
            }
        }

        int axis = 0;
        if (tMax.y < tMax[axis])
            axis = 1;
        if (tMax.z < tMax[axis])
            axis = 2;

        voxel[axis] += step[axis];
        t = tMax[axis];
        tMax[axis] += tDelta[axis];
    }

    return std::nullopt;
}
