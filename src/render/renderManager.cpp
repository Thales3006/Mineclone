#include "render/renderManager.h"

#include "render/renderLib.h"

#include <iostream>

RenderManager::RenderManager() {}

RenderManager::RenderManager(WindowManager *windowManager, World *world)
    : windowManager(windowManager), world(world) {
    initOpenGL();

    camera.setMinMax(0.01f, 400.0f);
    camera.setFOV(glm::radians(90.0f));

    textures = {
        {"container_box", std::make_shared<Texture>("textures/container.jpg")},
        {"blocks", std::make_shared<Texture>("textures/blocks_01.png")},
    };

    shaders = {
        {"terrain", std::make_shared<Shader>("shaders/shader.vert",
                                             "shaders/shader.frag")},
    };
}

void RenderManager::initOpenGL() {

    if (!gladLoadGL()) {
        std::cout << "Failed to initialize Glad.\n";
        return;
    }

    glm::uvec2 windowSize = windowManager->getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS);

    // No translucent blocks yet
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderManager::renderFrame() {
    glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::uvec2 windowSize = windowManager->getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);

    Player &player = world->getEntityManager()->player;
    camera.position =
        player.position +
        glm::vec3(player.size.x / 2, player.size.y * 0.9, player.size.z / 2);
    camera.direction = player.direction;

    float ratio =
        static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    shaders["terrain"]->setMat4("projection",
                                camera.getMatrixProjection(ratio));
    shaders["terrain"]->setMat4("view", camera.getMatrixView());

    renderChunks(player.chunkx, player.chunkz);
}

void RenderManager::renderChunks(int chunkx, int chunkz) {
    for (auto &chunkView : chunkViews) {
        chunkView->render(chunkx, chunkz);
    }
}

void RenderManager::updateView() {
    auto [mutex, chunks] = world->getChunkManager()->unsafe_getChunkMap();
    std::lock_guard<std::mutex> lock(*mutex);

    chunkViews.clear();
    for (auto &[coord, chunk] : *chunks) {
        chunkViews.push_back(std::make_unique<ChunkView>(
            chunk, std::vector{textures["blocks"]}, shaders["terrain"]));
    }
}
