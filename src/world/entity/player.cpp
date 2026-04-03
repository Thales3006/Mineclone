#include "world/entity/player.h"

Player::Player(glm::vec3 position, glm::vec3 size)
    : Entity(position, size, glm::vec3(0.0f, 0.0f, 1.0f)), hotbarIndex(7) {}

void Player::updatePlayer(ChunkManager &chunkManager, float deltaTime) {
    update(chunkManager, deltaTime);

    if (position.y < -16) {
        position.y = 32;
        velocity.y = 0;
    }
}
