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
    Player &player = game.world.getEntityManager()->player;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.direction);
        glm::vec3 i = glm::vec3(0);
        while (game.world.getChunkManager()
                   ->getBlock(player.chunkx, player.chunkz, player.position + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(1);
        }
        game.world.getChunkManager()->setBlock(player.chunkx, player.chunkz,
                                               player.position + dir * i, Block());
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.direction);
        glm::vec3 i = glm::vec3(0);
        while (game.world.getChunkManager()
                   ->getBlock(player.chunkx, player.chunkz, player.position + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(1);
        }
        i -= glm::vec3(1);

        if (!Entity::colision(
                player.position + dir * i, glm::vec3(0, 0, 0), glm::floor(player.position),
                glm::ceil(player.position + player.size) - glm::floor(player.position)))
            game.world.getChunkManager()->setBlock(player.chunkx, player.chunkz,
                                                   player.position + dir * i, Block(1, true));
    }
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));

    game.inputManager.handleKeyboardCallback(window, key, scancode, action, mods);
}
