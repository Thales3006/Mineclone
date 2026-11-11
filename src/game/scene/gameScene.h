#ifndef GAME_SCENE_CLASS_H
#define GAME_SCENE_CLASS_H

#include "game/input/inputManager.h"
#include "game/scene/scene.h"
#include "game/window.h"
#include "render/gameView.h"
#include "world/world.h"

#include <atomic>
#include <thread>

class GameScene : public Scene {
  public:
    GameScene(Window *window);
    ~GameScene();

    void onEnter() override;
    void onExit() override;

    void update(float dt) override;
    void render(Renderer &renderer) override;

    void onMouseMove(double dx, double dy) override;
    void onMouseButton(GLFWwindow *window, int button, int action,
                       int mods) override;
    void onKey(GLFWwindow *window, int key, int scancode, int action,
               int mods) override;

  private:
    World world;
    GameView gameView;
    InputManager input;

    std::atomic<bool> loadingChunks;
    std::thread chunkLoader;

    void autoLoadChunks();
};

#endif
