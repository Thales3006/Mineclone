#include "render/gameView.h"

#include "render/renderLib.h"

#include <algorithm>

GameView::GameView() {}

GameView::GameView(World *world) : world(world) {
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

void GameView::render(Renderer &renderer) {
    executeTasks();

    glm::uvec2 windowSize = renderer.getWindow()->getWindowSize();

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

void GameView::renderChunks(int chunkx, int chunkz) {
    for (auto &chunkView : chunkViews) {
        chunkView->render(chunkx, chunkz);
    }
}

void GameView::addChunkView(ChunkRegion region) {
    chunkViews.push_back(std::make_unique<ChunkView>(
        region, *terrainVertexMap, textureManager, shaders["terrain"]));
}

void GameView::removeChunkView(ChunkRegion region) {
    auto it = std::remove_if(chunkViews.begin(), chunkViews.end(),
                             [&](const std::unique_ptr<ChunkView> &chunkView) {
                                 return chunkView->region.main == region.main;
                             });
    if (it != chunkViews.end()) {
        chunkViews.erase(it, chunkViews.end());
    }
}

void GameView::updateChunkView(ChunkRegion region) {
    removeChunkView(region);
    addChunkView(region);
}

void GameView::executeTasks() {
    std::lock_guard<std::mutex> lock(taskMutex);

    while (!pendingTasks.empty()) {
        auto task = pendingTasks.front();
        task();
        pendingTasks.pop();
    }
}
