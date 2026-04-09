#include "world/entity/entity.h"

#include <bits/stdc++.h>

Entity::Entity()
    : Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
             glm::vec3(0.0f, 0.0f, 1.0f)) {}

Entity::Entity(glm::vec3 position, glm::vec3 size, glm::vec3 direction)
    : position(position), size(size), direction(direction), chunkx(0),
      chunkz(0), velocity(0.0f), acceleration(32.0, 7.0, 32.0), gravity(20),
      onGround(false) {}

typedef struct {
    Block block;
    float dist;
    glm::vec3 pos;
} blockDist;

void Entity::update(ChunkManager &chunkManager, float deltaTime) {
    if (chunkManager.isEmpty(chunkx, chunkz)) {
        return;
    }

    glm::vec3 friction = glm::vec3(8.0, 0.1, 8.0);
    glm::vec3 correction = glm::vec3(0.0);
    std::vector<blockDist> blocks;

    glm::vec3 nV = glm::vec3(velocity.x >= 0 ? 1 : -1, velocity.y >= 0 ? 1 : -1,
                             velocity.z >= 0 ? 1 : -1);
    glm::vec3 nSize = glm::vec3(velocity.x >= 0 ? 0 : -ceil(size.x),
                                velocity.y >= 0 ? 0 : -ceil(size.z),
                                velocity.z >= 0 ? 0 : -ceil(size.y));
    glm::vec3 dV = velocity * deltaTime + nSize;

    for (int x = 0; abs(x) <= abs(dV.x) + abs(size.x) + 1; x += nV.x) {
        for (int y = 0; abs(y) <= abs(dV.y) + abs(size.y) + 1; y += nV.y) {
            for (int z = 0; abs(z) <= abs(dV.z) + abs(size.z) + 1; z += nV.z) {
                const glm::vec3 colisionPos =
                    glm::floor(position) + glm::vec3(x, y, z) - nSize;
                const Block &block =
                    chunkManager.getBlock(chunkx, chunkz, colisionPos);

                if (block.solid == false)
                    continue;
                blocks.push_back(
                    blockDist{block,
                              glm::length((colisionPos + glm::vec3(0.5f)) -
                                          (position + size * 0.5f)),
                              colisionPos});
            }
        }
    }

    sort(blocks.begin(), blocks.end(),
         [](const blockDist &b1, const blockDist &b2) {
             return b1.dist <= b2.dist;
         });

    for (const blockDist &block : blocks) {
        glm::vec3 newCorrection =
            colisionContinuous(block.pos, glm::vec3(1.0f), deltaTime);
        if (newCorrection.x != 0 && correction.x == 0) {
            correction.x = newCorrection.x;
            position.x += velocity.x * deltaTime + correction.x * 1.001f;
            velocity.x = 0;
        }
        if (newCorrection.y != 0 && correction.y == 0) {
            correction.y = newCorrection.y;
            position.y += velocity.y * deltaTime + correction.y * 1.001f;
            velocity.y = 0;
        }
        if (newCorrection.z != 0 && correction.z == 0) {
            correction.z = newCorrection.z;
            position.z += velocity.z * deltaTime + correction.z * 1.001f;
            velocity.z = 0;
        }
    }
    if (correction.x == 0)
        position.x += velocity.x * deltaTime;
    if (correction.y == 0)
        position.y += velocity.y * deltaTime;
    if (correction.z == 0)
        position.z += velocity.z * deltaTime;

    onGround = correction.y > 0 ? true : false;
    if (!onGround)
        velocity.y -= gravity * deltaTime;

    velocity -= velocity * friction * glm::vec3(deltaTime);

    glm::vec3 chunkOffset = Chunk::chunkOffSet(position);

    chunkx += chunkOffset.x;
    chunkz += chunkOffset.z;

    position -= chunkOffset * float(CHUNK_WIDTH);
}

glm::vec3 Entity::colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize,
                                     float deltaTime) {
    if (velocity == glm::vec3(0.0))
        return glm::vec3(0.0);

    glm::vec3 deltaVel = velocity * deltaTime;

    glm::vec3 near = ((staticPos - size) - Entity::position) / deltaVel;
    glm::vec3 far =
        ((staticPos - size) + (staticSize + size) - Entity::position) /
        deltaVel;

    if (near.x > far.x)
        std::swap(near.x, far.x);
    if (near.y > far.y)
        std::swap(near.y, far.y);
    if (near.z > far.z)
        std::swap(near.z, far.z);

    if (near.x > far.y || near.x > far.z || near.y > far.x || near.y > far.z ||
        near.z > far.x || near.z > far.y)
        return glm::vec3(0.0);

    float nearTime = near.x > near.y ? (std::isnan(near.x) ? -1 : near.x)
                                     : (std::isnan(near.y) ? -1 : near.y);
    nearTime =
        nearTime > near.z ? nearTime : (std::isnan(near.z) ? -1 : near.z);

    if (nearTime < 0 || nearTime > 1)
        return glm::vec3(0.0);

    glm::vec3 normal;
    near.x > near.y   ? near.x > near.z ? normal = glm::vec3(-1, 0, 0)
                                        : normal = glm::vec3(0, 0, -1)
      : near.y > near.z ? normal = glm::vec3(0, -1, 0)
                      : normal = glm::vec3(0, 0, -1);

    return (deltaVel * normal) * (1 - nearTime);

    return (deltaVel * normal) * (1 - nearTime);
}

bool Entity::colision(glm::vec3 entity1Pos, glm::vec3 entity1size,
                      glm::vec3 entity2Pos, glm::vec3 entity2size) {
    return entity1Pos.x <= entity2Pos.x + entity2size.x &&
           entity1Pos.x + entity1size.x >= entity2Pos.x &&
           entity1Pos.y <= entity2Pos.y + entity2size.y &&
           entity1Pos.y + entity1size.y >= entity2Pos.y &&
           entity1Pos.z <= entity2Pos.z + entity2size.z &&
           entity1Pos.z + entity1size.z >= entity2Pos.z;
}
