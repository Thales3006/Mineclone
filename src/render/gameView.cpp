#include "render/gameView.h"

#include "game/task/taskManager.h"
#include "render/renderLib.h"

#include <algorithm>

GameView::GameView(World &world, Renderer &renderer)
    : renderer(renderer), world(world) {
    camera.setMinMax(0.01f, 400.0f);
    camera.setFOV(glm::radians(90.0f));

    textureManager = std::make_shared<TextureManager>();
    shaders = {
        {"terrain", std::make_shared<Shader>("shaders/shader.vert",
                                             "shaders/shader.frag")},
    };
    terrainVertexMap = std::make_shared<TerrainVertexMap>(textureManager);

    world.getChunkManager().onChunkAdded.connect([&](RegionSnapshot snapshot) {
        TaskManager::instance().run([this, snapshot = std::move(snapshot)]() {
            ChunkMeshData data =
                ChunkView::buildMeshData(snapshot, *terrainVertexMap);
            this->renderer.runOnMain([this, data = std::move(data)]() mutable {
                addChunkView(std::move(data));
            });
        });
    });

    world.getChunkManager().onChunkRemoved.connect([&](int x, int z) {
        this->renderer.runOnMain([this, x, z]() { removeChunkView(x, z); });
    });

    world.getChunkManager().onChunkUpdated.connect(
        [&](RegionSnapshot snapshot, glm::ivec3 worldEdit) {
            this->renderer.runOnMain(
                [this, snapshot = std::move(snapshot), worldEdit]() {
                    updateChunkView(snapshot, worldEdit);
                });
        });

    firstPersonView = std::make_unique<FirstPersonView>(
        world.getEntityManager().player, world, *terrainVertexMap,
        textureManager->getTexture("blocks"), shaders["terrain"]);
}

void GameView::render() {
    glm::uvec2 viewportSize = renderer.getViewportSize();

    Player &player = *world.getEntityManager().player;
    camera.position = player.eyePosition();
    camera.direction = player.direction;

    float ratio = static_cast<float>(viewportSize.x) /
                  static_cast<float>(viewportSize.y);
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

void GameView::addChunkView(ChunkMeshData data) {
    for (auto &chunkView : chunkViews)
        if (chunkView->x == data.x && chunkView->z == data.z) {
            chunkView = std::make_unique<ChunkView>(std::move(data),
                                                    *terrainVertexMap,
                                                    textureManager,
                                                    shaders["terrain"]);
            return;
        }

    chunkViews.push_back(std::make_unique<ChunkView>(
        std::move(data), *terrainVertexMap, textureManager, shaders["terrain"]));
}

void GameView::removeChunkView(int x, int z) {
    auto it = std::remove_if(chunkViews.begin(), chunkViews.end(),
                             [&](const std::unique_ptr<ChunkView> &chunkView) {
                                 return chunkView->x == x && chunkView->z == z;
                             });
    if (it != chunkViews.end()) {
        chunkViews.erase(it, chunkViews.end());
    }
}

void GameView::updateChunkView(const RegionSnapshot &snapshot,
                               glm::ivec3 worldEdit) {
    int cx = snapshot.main->x;
    int cz = snapshot.main->z;

    for (auto &chunkView : chunkViews) {
        if (chunkView->x == cx && chunkView->z == cz) {
            chunkView->updateBlock(snapshot, worldEdit);
            return;
        }
    }

    addChunkView(ChunkView::buildMeshData(snapshot, *terrainVertexMap));
}
