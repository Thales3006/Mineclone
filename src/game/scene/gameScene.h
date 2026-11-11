#ifndef GAME_SCENE_CLASS_H
#define GAME_SCENE_CLASS_H

#include "game/input/inputBus.h"
#include "game/scene/scene.h"
#include "render/gameView.h"
#include "world/world.h"

#include <atomic>
#include <thread>

class GameScene : public Scene {
  public:
    GameScene(InputBus &bus);
    ~GameScene();

    void onEnter() override;
    void onExit() override;

    void update(float dt) override;
    void render(Renderer &renderer) override;

    void onEvent(const InputEvent &e) override;

  private:
    World world;
    GameView gameView;
    InputBus &bus;

    float sensibility;

    std::atomic<bool> loadingChunks;
    std::thread chunkLoader;

    void autoLoadChunks();

    void applyMovement(float dt);
    void triggerAction(Action action);
    void destroyLookedBlock();
    void placeLookedBlock();
};

#endif
