#include "world/entity/entityManager.h"

EntityManager::EntityManager()
    : player(glm::vec3(5.0f, 40.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5)) {}

void EntityManager::update(ChunkManager &chunkManager, float deltaTime) {
    player.updatePlayer(chunkManager, deltaTime);
}
