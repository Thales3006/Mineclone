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

    world.getChunkManager().onChunkRemoved.connect([&](glm::ivec2 coord) {
        this->renderer.runOnMain([this, coord]() { removeChunkView(coord); });
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

    renderChunks(player.chunk);
    firstPersonView->render(player.chunk);
}

void GameView::renderChunks(glm::ivec2 chunk) {
    for (auto &chunkView : chunkViews) {
        chunkView->render(chunk);
    }
}

void GameView::addChunkView(ChunkMeshData data) {
    for (auto &chunkView : chunkViews)
        if (chunkView->coord == data.coord) {
            chunkView = std::make_unique<ChunkView>(std::move(data),
                                                    *terrainVertexMap,
                                                    textureManager,
                                                    shaders["terrain"]);
            return;
        }

    chunkViews.push_back(std::make_unique<ChunkView>(
        std::move(data), *terrainVertexMap, textureManager, shaders["terrain"]));
}

void GameView::removeChunkView(glm::ivec2 coord) {
    auto it = std::remove_if(chunkViews.begin(), chunkViews.end(),
                             [&](const std::unique_ptr<ChunkView> &chunkView) {
                                 return chunkView->coord == coord;
                             });
    if (it != chunkViews.end()) {
        chunkViews.erase(it, chunkViews.end());
    }
}

void GameView::updateChunkView(const RegionSnapshot &snapshot,
                               glm::ivec3 worldEdit) {
    glm::ivec2 coord = snapshot.main->coord;

    for (auto &chunkView : chunkViews) {
        if (chunkView->coord == coord) {
            chunkView->updateBlock(snapshot, worldEdit);
            return;
        }
    }

    addChunkView(ChunkView::buildMeshData(snapshot, *terrainVertexMap));
}
