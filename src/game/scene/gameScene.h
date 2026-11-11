#ifndef GAME_SCENE_CLASS_H
#define GAME_SCENE_CLASS_H

#include "game/scene/scene.h"
#include "world/world.h"
#include "render/renderer.h"

class GameScene: public Scene {
  public:

    GameScene();

    void update(float dt) override;
    void render(Renderer &renderer) override;

  private:
    World world;
};

#endif
