#include "game/game.h"

#include <thread>

Game::Game() : world(42) {
    windowManager.createWindow(this);

    renderManager = RenderManager(&windowManager, &world);
    deltaTime = 0;
    glfwSetTime(0);
}

void Game::autoLoadChunks() {
    Player *player = &world.getEntityManager()->player;
    while (isRunning) {
        world.getChunkManager()->fillChunkRadius(6, player->chunkx, player->chunkz);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Game::run() {
    std::thread chunkLoader(&Game::autoLoadChunks, this);

    while (!glfwWindowShouldClose(windowManager.getWindow())) {
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        world.getEntityManager()->update(windowManager, *world.getChunkManager(), deltaTime);

        renderManager.renderFrame();

        // std::cout << "FPS: "<< 1/deltaTime <<" \n";

        glfwSwapBuffers(windowManager.getWindow());
        glfwPollEvents();
    }

    isRunning = false;
    chunkLoader.join();

    glfwTerminate();
}
