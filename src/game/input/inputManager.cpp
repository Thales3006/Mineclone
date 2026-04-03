#include "game/input/inputManager.h"

InputManager::InputManager() {}

InputManager::InputManager(World *world, WindowManager *windowManager,
                           RenderManager *renderManager)
    : world(world), windowManager(windowManager), renderManager(renderManager),
      sensibility(0.005f) {}

void InputManager::processKeyboard(float deltaTime) {

    Player &player = world->getEntityManager()->player;
    glm::vec3 front =
        glm::normalize(glm::vec3(player.direction.x, 0, player.direction.z));
    glm::vec3 side = glm::normalize(
        glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), player.direction));
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
            renderManager->camera.setFOV(renderManager->camera.getFOV() +
                                         glm::radians(1.0));
            break;
        case DecreaseFOV:
            renderManager->camera.setFOV(renderManager->camera.getFOV() -
                                         glm::radians(1.0));
            break;
        default:
            break;
        }
    }

    if (newDir != glm::vec3(0.0f))
        player.velocity += glm::normalize(newDir) *
                           (player.acceleration * glm::vec3(deltaTime));

    // if (glfwGetKey(window, down_key)){
    //     velocity -= up * (acceleration * deltaTime);
    // }
}

void InputManager::handleKeyboardCallback(GLFWwindow *window, int key,
                                          int scancode, int keyAction,
                                          int mods) {
    if (Action action = inputMap.getInput(key); action != None) {
        if (keyAction == GLFW_PRESS) {
            activeActions.insert(action);
            handleSingleAction(window, action);
        } else if (keyAction == GLFW_RELEASE) {
            activeActions.erase(action);
        }
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
        glm::vec3 right =
            glm::normalize(glm::cross(player.direction, glm::vec3(0, 1, 0)));
        rotMat = glm::rotate(glm::mat4(1.0f), rotY, right);
        player.direction =
            glm::vec3(rotMat * glm::vec4(player.direction, 0.0f));
    }

    player.direction = glm::normalize(player.direction);
}

void InputManager::handleMouseClickCallback(GLFWwindow *window, int button,
                                            int mouseAction, int mods) {
    if (Action action = inputMap.getInput(button); action != None) {
        if (mouseAction == GLFW_PRESS) {
            activeActions.insert(action);
            handleSingleAction(window, action);
        } else if (mouseAction == GLFW_RELEASE) {
            activeActions.erase(action);
        }
    }
}

void InputManager::handleSingleAction(GLFWwindow *window, Action action) {
    static bool fullScreen = false;
    const GLFWvidmode *mode = glfwGetVideoMode(windowManager->getMonitor());

    Player &player = world->getEntityManager()->player;
    glm::vec3 initial_pos =
        player.position + player.size * glm::vec3(0.5f, 0.9f, 0.5f);
    glm::vec3 dir = glm::normalize(player.direction);
    glm::vec3 i = glm::vec3(0);

    switch (action) {

    case Fullscreen:
        if (!fullScreen) {
            glfwSetWindowMonitor(window, windowManager->getMonitor(), 0, 0,
                                 mode->width, mode->height, mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height);
            fullScreen = true;
        } else {
            glfwSetWindowMonitor(window, 0, mode->width / 2 - 1200 / 2,
                                 mode->height / 2 - 600 / 2, 1200, 600,
                                 mode->refreshRate);
            glViewport(0, 0, 1200, 600);
            fullScreen = false;
        }
        break;
    case DestroyBlock:
        while (
            world->getChunkManager()
                ->getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
                .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(0.25);
        }
        world->getChunkManager()->setBlock(player.chunkx, player.chunkz,
                                           initial_pos + dir * i, Block());
        break;
    case PlaceBlock:
        while (
            world->getChunkManager()
                ->getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
                .ID == 0) {
            if (i.x > 6)
                return;
            i += glm::vec3(0.25);
        }
        i -= glm::vec3(0.25);

        if (!Entity::colision(initial_pos + dir * i, glm::vec3(0, 0, 0),
                              glm::floor(player.position),
                              glm::ceil(player.position + player.size) -
                                  glm::floor(player.position))) {
            world->getChunkManager()->setBlock(player.chunkx, player.chunkz,
                                               initial_pos + dir * i,
                                               Block(player.hotbarIndex, true));
        }
        break;

    case HotbarSlot1:
        player.hotbarIndex = 1;
        break;
    case HotbarSlot2:
        player.hotbarIndex = 2;
        break;
    case HotbarSlot3:
        player.hotbarIndex = 3;
        break;
    case HotbarSlot4:
        player.hotbarIndex = 4;
        break;
    case HotbarSlot5:
        player.hotbarIndex = 5;
        break;
    case HotbarSlot6:
        player.hotbarIndex = 6;
        break;
    case HotbarSlot7:
        player.hotbarIndex = 7;
        break;
    case HotbarSlot8:
        player.hotbarIndex = 8;
        break;
    case HotbarSlot9:
        player.hotbarIndex = 9;
        break;
    default:
        break;
    }
}
