#include "game/game.h"

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.inputManager.handleMouseMovementCallback(xpos - xlast, ylast - ypos);
    xlast = xpos;
    ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.inputManager.handleMouseClickCallback(window, button, action, mods);
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    game.inputManager.handleKeyboardCallback(window, key, scancode, action, mods);
}
