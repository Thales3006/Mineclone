#include "world/world.h"

World::World(int seed) : seed(seed) {}

ChunkManager *World::getChunkManager() { return &chunkManager; }

EntityManager *World::getEntityManager() { return &entityManager; }

void World::updateTick(WindowManager &windowManager, float deltaTime) {
    entityManager.update(windowManager, chunkManager, deltaTime);
}
