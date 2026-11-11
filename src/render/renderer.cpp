#include "render/renderer.h"

#include "render/renderLib.h"

#include <algorithm>
#include <iostream>

Renderer::Renderer() {}

Renderer::Renderer(Window *windowManager,
                             std::shared_ptr<World> world)
    : windowManager(windowManager), world(world) {
    initOpenGL();

    camera.setMinMax(0.01f, 400.0f);
    camera.setFOV(glm::radians(90.0f));

    textureManager = std::make_shared<TextureManager>();
    shaders = {
        {"terrain", std::make_shared<Shader>("shaders/shader.vert",
                                             "shaders/shader.frag")},
    };
    terrainVertexMap = std::make_shared<TerrainVertexMap>(textureManager);

    world->getChunkManager()->onChunkAdded.connect([&](ChunkRegion region) {
        std::lock_guard<std::mutex> lock(taskMutex);
        pendingTasks.push([this, region]() { addChunkView(region); });
    });

    world->getChunkManager()->onChunkRemoved.connect([&](ChunkRegion region) {
        std::lock_guard<std::mutex> lock(taskMutex);
        pendingTasks.push([this, region]() { removeChunkView(region); });
    });

    world->getChunkManager()->onChunkUpdated.connect([&](ChunkRegion region) {
        std::lock_guard<std::mutex> lock(taskMutex);
        pendingTasks.push([this, region]() { updateChunkView(region); });
    });

    firstPersonView = std::make_unique<FirstPersonView>(
        world->getEntityManager()->player, world, *terrainVertexMap,
        textureManager->getTexture("blocks"), shaders["terrain"]);
}

void Renderer::initOpenGL() {

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

void Renderer::renderFrame() {
    excuteTasks();

    glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::uvec2 windowSize = windowManager->getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);

    Player &player = *world->getEntityManager()->player;
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
    firstPersonView->render(player.chunkx, player.chunkz);
}

void Renderer::renderChunks(int chunkx, int chunkz) {
    for (auto &chunkView : chunkViews) {
        chunkView->render(chunkx, chunkz);
    }
}

void Renderer::addChunkView(ChunkRegion region) {
    chunkViews.push_back(std::make_unique<ChunkView>(
        region, *terrainVertexMap, textureManager, shaders["terrain"]));
}

void Renderer::removeChunkView(ChunkRegion region) {
    auto it = std::remove_if(chunkViews.begin(), chunkViews.end(),
                             [&](const std::unique_ptr<ChunkView> &chunkView) {
                                 return chunkView->region.main == region.main;
                             });
    if (it != chunkViews.end()) {
        chunkViews.erase(it, chunkViews.end());
    }
}
void Renderer::updateChunkView(ChunkRegion region) {
    removeChunkView(region);
    addChunkView(region);
}

void Renderer::excuteTasks() {
    std::lock_guard<std::mutex> lock(taskMutex);

    while (!pendingTasks.empty()) {
        auto task = pendingTasks.front();
        task();
        pendingTasks.pop();
    }
}
