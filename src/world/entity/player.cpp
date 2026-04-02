#include "world/entity/player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player(glm::vec3 position, glm::vec3 size) : Entity(position, size) {}

void Player::updatePlayer(ChunkManager &chunkManager, float deltaTime) {
    update(chunkManager, deltaTime);

    if (position.y < -16) {
        position.y = 32;
        velocity.y = 0;
    }
}
