#include "game/game.h"

#include "game/scene/gameScene.h"

Game::Game() : deltaTime(0), window(), renderer(window), bus(inputMap) {
    window.setUserPointer(this);
    scene = std::make_unique<GameScene>(bus);
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

void Game::onEvent(const InputEvent &e) {
    if (auto k = std::get_if<KeyEvent>(&e);
        k && k->action == GLFW_PRESS && bus.actionFor(e) == Fullscreen)
        window.toggleFullscreen();

    scene->onEvent(e);
}

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.onEvent(game.bus.feedMouseMove(xpos - xlast, ypos - ylast));
    xlast = xpos;
    ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action,
                              int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    game.onEvent(game.bus.feedButton(button, action, mods, x, y));
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.onEvent(game.bus.feedKey(key, scancode, action, mods));
}
