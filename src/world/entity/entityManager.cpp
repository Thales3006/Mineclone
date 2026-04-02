#include "world/entity/entityManager.h"

EntityManager::EntityManager() : player(glm::vec3(5.0f, 20.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5)) {
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE,
                   GLFW_KEY_LEFT_SHIFT);
    player.camera.setMinMax(0.01f, 400.0f);
    player.camera.setFOV(glm::radians(90.0f));
}

void EntityManager::update(WindowManager &windowManager, ChunkManager &chunkManager,
                           float deltaTime) {
    player.updatePlayer(windowManager.getWindow(), chunkManager, deltaTime);
}
