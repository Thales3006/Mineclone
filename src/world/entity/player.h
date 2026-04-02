#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "render/camera.h"
#include "world/entity/entity.h"

#include "render/renderLib.h"
#include <glm/glm.hpp>

#include <vector>

class Player : public Entity {
  public:
    Player(glm::vec3 pos, glm::vec3 siz);

    void updatePlayer(ChunkManager &chunkManager, float deltaTime);
};

#endif
