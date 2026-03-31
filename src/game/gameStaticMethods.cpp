#include "game/game.h"

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.player.processMouseMovement(xpos - xlast, ylast - ypos);
    xlast = xpos;
    ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    Player &player = game.player;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.camera.getDirection());
        glm::vec3 i = glm::vec3(0);
        while (game.chunkManager
                   .getBlock(player.chunkx, player.chunkz, player.camera.position + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(1);
        }
        game.chunkManager.setBlock(player.chunkx, player.chunkz, player.camera.position + dir * i,
                                   Block());
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.camera.getDirection());
        glm::vec3 i = glm::vec3(0);
        while (game.chunkManager
                   .getBlock(player.chunkx, player.chunkz, player.camera.position + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(1);
        }
        i -= glm::vec3(1);

        if (!Entity::colision(
                player.camera.position + dir * i, glm::vec3(0, 0, 0), glm::floor(player.position),
                glm::ceil(player.position + player.size) - glm::floor(player.position)))
            game.chunkManager.setBlock(player.chunkx, player.chunkz,
                                       player.camera.position + dir * i, Block(1, true));
    }
}

void Game::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    const GLFWvidmode *mode = glfwGetVideoMode(game.monitor);

    static bool fullScreen = false;
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        if (!fullScreen) {
            glfwSetWindowMonitor(window, game.monitor, 0, 0, mode->width, mode->height,
                                 mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height);
            fullScreen = true;
        } else {
            glfwSetWindowMonitor(window, 0, mode->width / 2 - 1200 / 2, mode->height / 2 - 600 / 2,
                                 1200, 600, mode->refreshRate);
            glViewport(0, 0, 1200, 600);
            fullScreen = false;
        }
    }
}
