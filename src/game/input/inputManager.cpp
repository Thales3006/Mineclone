#include "game/input/inputManager.h"

InputManager::InputManager() {}

InputManager::InputManager(World *world, WindowManager *windowManager, RenderManager *renderManager)
    : world(world), windowManager(windowManager), renderManager(renderManager),
      sensibility(0.005f) {}

void InputManager::processKeyboard(float deltaTime) {

    Player &player = world->getEntityManager()->player;
    glm::vec3 front = glm::normalize(glm::vec3(player.direction.x, 0, player.direction.z));
    glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), player.direction));
    glm::vec3 newDir = glm::vec3(0.0f);

    for (const auto &action : activeActions) {
        switch (action) {
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
        case IncreaseFOV:
            renderManager->camera.setFOV(renderManager->camera.getFOV() + glm::radians(1.0));
            break;
        case DecreaseFOV:
            renderManager->camera.setFOV(renderManager->camera.getFOV() - glm::radians(1.0));
            break;
        default:
            break;
        }
    }

    if (newDir != glm::vec3(0.0f))
        player.velocity += glm::normalize(newDir) * (player.acceleration * glm::vec3(deltaTime));

    // if (glfwGetKey(window, down_key)){
    //     velocity -= up * (acceleration * deltaTime);
    // }
}

void InputManager::handleKeyboardCallback(GLFWwindow *window, int key, int scancode, int keyAction,
                                          int mods) {
    if (Action action = inputMap.getInput(key); action != None) {
        if (keyAction == GLFW_PRESS) {
            activeActions.insert(action);
        } else if (keyAction == GLFW_RELEASE) {
            activeActions.erase(action);
        }
    }

    if (keyAction != GLFW_PRESS) {
        return;
    }
    static bool fullScreen = false;
    const GLFWvidmode *mode = glfwGetVideoMode(windowManager->getMonitor());

    switch (inputMap.getInput(key)) {

    case Fullscreen:
        if (!fullScreen) {
            glfwSetWindowMonitor(window, windowManager->getMonitor(), 0, 0, mode->width,
                                 mode->height, mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height);
            fullScreen = true;
        } else {
            glfwSetWindowMonitor(window, 0, mode->width / 2 - 1200 / 2, mode->height / 2 - 600 / 2,
                                 1200, 600, mode->refreshRate);
            glViewport(0, 0, 1200, 600);
            fullScreen = false;
        }
        break;
    default:
        break;
    }
}

void InputManager::handleMouseMovementCallback(double xoffset, double yoffset) {
    Player &player = world->getEntityManager()->player;
    float rotX = -xoffset * sensibility;
    float rotY = yoffset * sensibility;

    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(0, 1, 0));
    player.direction = glm::vec3(rotMat * glm::vec4(player.direction, 0.0f));

    float currentPitch = asin(player.direction.y);
    float maxPitch = glm::radians(89.9f);
    float newPitch = currentPitch + rotY;

    if (newPitch > -maxPitch && newPitch < maxPitch) {
        glm::vec3 right = glm::normalize(glm::cross(player.direction, glm::vec3(0, 1, 0)));
        rotMat = glm::rotate(glm::mat4(1.0f), rotY, right);
        player.direction = glm::vec3(rotMat * glm::vec4(player.direction, 0.0f));
    }

    player.direction = glm::normalize(player.direction);
}

void InputManager::handleMouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    Player &player = world->getEntityManager()->player;

    glm::vec3 initial_pos = player.position + player.size * glm::vec3(0.5f, 0.9f, 0.5f);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.direction);
        glm::vec3 i = glm::vec3(0);
        while (world->getChunkManager()
                   ->getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(0.25);
        }
        world->getChunkManager()->setBlock(player.chunkx, player.chunkz, initial_pos + dir * i,
                                           Block());
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.direction);
        glm::vec3 i = glm::vec3(0);
        while (world->getChunkManager()
                   ->getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
                   .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(0.25);
        }
        i -= glm::vec3(0.25);

        if (!Entity::colision(
                initial_pos + dir * i, glm::vec3(0, 0, 0), glm::floor(player.position),
                glm::ceil(player.position + player.size) - glm::floor(player.position))) {
            world->getChunkManager()->setBlock(player.chunkx, player.chunkz, initial_pos + dir * i,
                                               Block(7, true));
        }
    }
}
