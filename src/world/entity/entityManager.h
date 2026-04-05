#ifndef ENTITY_MANAGER_CLASS
#define ENTITY_MANAGER_CLASS

#include "world/entity/entity.h"
#include "world/entity/player.h"

#include "game/windowManager.h"

class EntityManager {
  public:
    EntityManager();

    std::shared_ptr<Player> player;

    void update(ChunkManager &chunkManager, float deltaTime);

  private:
    // std::vector<Entity> entities;
};

#endif
