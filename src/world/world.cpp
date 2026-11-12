#include "world/world.h"

World::World(int seed) : seed(seed), chunkManager(seed) {}

ChunkManager &World::getChunkManager() { return chunkManager; }

EntityManager &World::getEntityManager() { return entityManager; }

void World::updateTick(float deltaTime) {
    entityManager.update(chunkManager, deltaTime);
    chunkManager.updateLoading(entityManager.player->chunk);
}
