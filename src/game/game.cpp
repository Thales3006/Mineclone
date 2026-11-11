#include "game/game.h"

#include "game/scene/gameScene.h"

Game::Game() : deltaTime(0), window(), renderer(window) {
    window.setUserPointer(this);
    scene = std::make_unique<GameScene>(&window);
    glfwSetTime(0);
}

void Game::run() {
    scene->onEnter();

    while (!glfwWindowShouldClose(window.getWindow())) {
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        scene->update(deltaTime);

        renderer.beginFrame();
        scene->render(renderer);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    scene->onExit();
    glfwTerminate();
}

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.scene->onMouseMove(xpos - xlast, ypos - ylast);
    xlast = xpos;
    ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action,
                              int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.scene->onMouseButton(window, button, action, mods);
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.scene->onKey(window, key, scancode, action, mods);
}
