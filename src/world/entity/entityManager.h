#ifndef ENTITY_MANAGER_CLASS
#define ENTITY_MANAGER_CLASS

#include "world/entity/entity.h"
#include "world/entity/player.h"

#include "game/windowManager.h"

class EntityManager {
  public:
    EntityManager();

    Player player;

    void update(WindowManager &windowManager, ChunkManager &chunkManager, float deltaTime);

  private:
    // std::vector<Entity> entities;
};

#endif
