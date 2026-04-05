#include "game/game.h"

#include <thread>

Game::Game()
    : isRunning(false), world(std::make_shared<World>(42)),
      windowManager(std::make_shared<WindowManager>()) {

    windowManager->createWindow(this);
    renderManager = std::make_shared<RenderManager>(windowManager, world);
    inputManager =
        std::make_unique<InputManager>(world, windowManager, renderManager);

    deltaTime = 0;
    glfwSetTime(0);
}

void Game::autoLoadChunks() {
    Player &player = world->getEntityManager()->player;
    while (isRunning) {
        world->getChunkManager()->fillChunkRadius(6, player.chunkx,
                                                  player.chunkz);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Game::run() {
    isRunning = true;
    std::thread chunkLoader(&Game::autoLoadChunks, this);

    while (!glfwWindowShouldClose(windowManager->getWindow())) {
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        inputManager->processKeyboard(deltaTime);

        world->updateTick(deltaTime);
        renderManager->renderFrame();

        // std::cout << "FPS: "<< 1/deltaTime <<" \n";

        glfwSwapBuffers(windowManager->getWindow());
        glfwPollEvents();
    }

    isRunning = false;
    chunkLoader.join();

    glfwTerminate();
}

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.inputManager->handleMouseMovementCallback(xpos - xlast, ypos - ylast);
    xlast = xpos;
    ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action,
                              int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.inputManager->handleMouseClickCallback(window, button, action, mods);
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.inputManager->handleKeyboardCallback(window, key, scancode, action,
                                              mods);
}
