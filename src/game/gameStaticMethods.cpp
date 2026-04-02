#include "game/game.h"

void Game::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
    static double xlast = xpos;
    static double ylast = ypos;
    game.processMouseMovement(xpos - xlast, ylast - ypos);
    xlast = xpos;
    ylast = ypos;
}

void Game::processMouseMovement(double xoffset, double yoffset) {
    float rotX = -xoffset * sensibility;
    float rotY = yoffset * sensibility;

    Player &player = world.getEntityManager()->player;

    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(0, 1, 0));
    player.direction = glm::vec3(rotMat * glm::vec4(player.direction, 0.0f));

    glm::vec3 right = glm::normalize(glm::cross(player.direction, glm::vec3(0, 1, 0)));
    rotMat = glm::rotate(glm::mat4(1.0f), rotY, right);

    player.direction = glm::vec3(rotMat * glm::vec4(player.direction, 0.0f));
    player.direction = glm::normalize(player.direction);
}

void Game::mouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    Game &game = *(static_cast<Game *>(glfwGetWindowUserPointer(window)));
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
    const GLFWvidmode *mode = glfwGetVideoMode(game.windowManager.getMonitor());

    static bool fullScreen = false;
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        if (!fullScreen) {
            glfwSetWindowMonitor(window, game.windowManager.getMonitor(), 0, 0, mode->width,
                                 mode->height, mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height);
            fullScreen = true;
        } else {
            glfwSetWindowMonitor(window, 0, mode->width / 2 - 1200 / 2, mode->height / 2 - 600 / 2,
                                 1200, 600, mode->refreshRate);
            glViewport(0, 0, 1200, 600);
            fullScreen = false;
        }
    }
    game.processKeyMovement(key);
}

void Game::processKeyMovement(int key) {
    Player &player = world.getEntityManager()->player;
    glm::vec3 front = glm::normalize(glm::vec3(player.direction.x, 0, player.direction.z));
    glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), player.direction));
    glm::vec3 newDir = glm::vec3(0.0f);

    switch (inputMap.getInput(key)) {
    case MoveFoward:
        newDir += front;
        break;
    case MoveBackward:
        newDir -= front;
        break;
    case MoveLeft:
        newDir += side;
        break;
    case MoveRight:
        newDir -= side;
        break;
    case Jump:
        if (player.onGround) {
            player.velocity.y = player.acceleration.y;
            player.onGround = false;
        }
        break;
    case None:
        break;
    }

    if (newDir != glm::vec3(0.0f))
        player.velocity += glm::normalize(newDir) * (player.acceleration * glm::vec3(deltaTime));

    // if (glfwGetKey(window, down_key)){
    //     velocity -= up * (acceleration * deltaTime);
    // }
    /*
        if (glfwGetKey(window, GLFW_KEY_Z)) {
            camera.setFOV(camera.getFOV() - glm::radians(1.0));
        }
        if (glfwGetKey(window, GLFW_KEY_X)) {
            camera.setFOV(camera.getFOV() + glm::radians(1.0));
        }
      */
}
