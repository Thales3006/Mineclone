#ifndef WORLD_CLASS
#define WORLD_CLASS

#include "world/entity/entityManager.h"
#include "world/terrain/chunk/chunkManager.h"

class World {
  public:
    World(int seed);
    ChunkManager *getChunkManager();
    EntityManager *getEntityManager();

  private:
    int seed;

    ChunkManager chunkManager;
    EntityManager entityManager;
};

#endif
