#include "game/game.h"

#include <thread>

Game::Game() {
    windowManager.createWindow(this);

    player = Player(glm::vec3(5.0f, 20.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5));
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE,
                   GLFW_KEY_LEFT_SHIFT);
    player.camera.setMinMax(0.01f, 400.0f);
    player.camera.setFOV(glm::radians(90.0f));

    renderManager = RenderManager(&windowManager, &chunkManager, &player);
    deltaTime = 0;
    glfwSetTime(0);
}

void Game::autoLoadChunks() {
    while (isRunning) {
        chunkManager.fillChunkRadius(6, player.chunkx, player.chunkz);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Game::run() {
    std::thread chunkLoader(&Game::autoLoadChunks, this);

    while (!glfwWindowShouldClose(windowManager.getWindow())) {
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        player.updatePlayer(windowManager.getWindow(), chunkManager, deltaTime);

        renderManager.renderFrame();

        // std::cout << "FPS: "<< 1/deltaTime <<" \n";

        glfwSwapBuffers(windowManager.getWindow());
        glfwPollEvents();
    }

    isRunning = false;
    chunkLoader.join();

    glfwTerminate();
}
