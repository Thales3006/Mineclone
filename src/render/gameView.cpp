#include "render/gameView.h"

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

    world.getChunkManager().onChunkAdded.connect([&](ChunkRegion region) {
        this->renderer.runOnMain([this, region]() { addChunkView(region); });
    });

    world.getChunkManager().onChunkRemoved.connect([&](ChunkRegion region) {
        this->renderer.runOnMain([this, region]() { removeChunkView(region); });
    });

    world.getChunkManager().onChunkUpdated.connect(
        [&](ChunkRegion region, glm::ivec3 worldEdit) {
            this->renderer.runOnMain([this, region, worldEdit]() {
                updateChunkView(region, worldEdit);
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

void GameView::addChunkView(ChunkRegion region) {
    chunkViews.push_back(std::make_unique<ChunkView>(
        region, *terrainVertexMap, textureManager, shaders["terrain"]));
}

void GameView::removeChunkView(ChunkRegion region) {
    auto it = std::remove_if(chunkViews.begin(), chunkViews.end(),
                             [&](const std::unique_ptr<ChunkView> &chunkView) {
                                 return chunkView->x == region.main->x &&
                                        chunkView->z == region.main->z;
                             });
    if (it != chunkViews.end()) {
        chunkViews.erase(it, chunkViews.end());
    }
}

void GameView::updateChunkView(ChunkRegion region, glm::ivec3 worldEdit) {
    for (auto &chunkView : chunkViews) {
        if (chunkView->x == region.main->x && chunkView->z == region.main->z) {
            chunkView->updateBlock(region, worldEdit);
            return;
        }
    }

    addChunkView(region);
}
