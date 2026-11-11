#include "game/scene/gameScene.h"

#include <chrono>

GameScene::GameScene(Window *window)
    : world(42), gameView(&world), input(&world, window, &gameView),
      loadingChunks(false) {}

GameScene::~GameScene() { onExit(); }

void GameScene::onEnter() {
    loadingChunks = true;
    chunkLoader = std::thread(&GameScene::autoLoadChunks, this);
}

void GameScene::onExit() {
    loadingChunks = false;
    if (chunkLoader.joinable())
        chunkLoader.join();
}

void GameScene::update(float dt) {
    input.processKeyboard(dt);
    world.updateTick(dt);
}

void GameScene::render(Renderer &renderer) { gameView.render(renderer); }

void GameScene::onMouseMove(double dx, double dy) {
    input.handleMouseMovementCallback(dx, dy);
}

void GameScene::onMouseButton(GLFWwindow *window, int button, int action,
                              int mods) {
    input.handleMouseClickCallback(window, button, action, mods);
}

void GameScene::onKey(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {
    input.handleKeyboardCallback(window, key, scancode, action, mods);
}

void GameScene::autoLoadChunks() {
    Player &player = *world.getEntityManager()->player;
    while (loadingChunks) {
        world.getChunkManager()->fillChunkRadius(6, player.chunkx,
                                                 player.chunkz);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
