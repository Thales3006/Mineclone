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

glm::vec3 Player::forward() const {
    return glm::normalize(glm::vec3(direction.x, 0, direction.z));
}

glm::vec3 Player::right() const {
    return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
}

glm::vec3 Player::eyePosition() const {
    return position + size * glm::vec3(0.5f, 0.9f, 0.5f);
}

void Player::move(glm::vec3 worldDir, float deltaTime) {
    if (worldDir == glm::vec3(0.0f))
        return;
    velocity +=
        glm::normalize(worldDir) * (acceleration * glm::vec3(deltaTime));
}

void Player::jump() {
    if (onGround) {
        velocity.y = acceleration.y;
        onGround = false;
    }
}

void Player::look(float dx, float dy, float sensibility) {
    float rotX = -dx * sensibility;
    float rotY = -dy * sensibility;

    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(0, 1, 0));
    direction = glm::vec3(rotMat * glm::vec4(direction, 0.0f));

    float currentPitch = asin(direction.y);
    float maxPitch = glm::radians(89.9f);
    float newPitch = currentPitch + rotY;

    if (newPitch > -maxPitch && newPitch < maxPitch) {
        glm::vec3 r = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
        rotMat = glm::rotate(glm::mat4(1.0f), rotY, r);
        direction = glm::vec3(rotMat * glm::vec4(direction, 0.0f));
    }

    direction = glm::normalize(direction);
}
