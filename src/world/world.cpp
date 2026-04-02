#include "world/world.h"

World::World(int seed) : seed(seed) {}

ChunkManager *World::getChunkManager() { return &chunkManager; }

EntityManager *World::getEntityManager() { return &entityManager; }

void World::updateTick(float deltaTime) { entityManager.update(chunkManager, deltaTime); }
