#include "render/renderManager.h"

#include "render/renderLib.h"

#include <iostream>

RenderManager::RenderManager() {}

RenderManager::RenderManager(WindowManager *windowManager, World *world)
    : windowManager(windowManager), world(world) {
    initOpenGL();

    camera.setMinMax(0.01f, 400.0f);
    camera.setFOV(glm::radians(90.0f));

    textures = {Texture("texture_diffuse", "textures/container.jpg"),
                Texture("texture_diffuse", "textures/blocks_opaque_01.png")};

    shaders.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));
}

void RenderManager::initOpenGL() {

    if (!gladLoadGL()) {
        std::cout << "Failed to initialize Glad.\n";
        return;
    }

    glm::uvec2 windowSize = windowManager->getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void RenderManager::renderFrame() {
    glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::uvec2 windowSize = windowManager->getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);

    Player &player = world->getEntityManager()->player;
    camera.position =
        player.position + glm::vec3(player.size.x / 2, player.size.y * 0.9, player.size.z / 2);
    camera.direction = player.direction;
    float ratio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    shaders[0].setMat4("projection", camera.getMatrixProjection(ratio));
    shaders[0].setMat4("view", camera.getMatrixView());

    world->getChunkManager()->renderChunks(shaders[0], player.chunkx, player.chunkz, textures);
}
