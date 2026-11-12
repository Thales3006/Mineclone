#include "render/gameView.h"

#include "game/task/taskManager.h"
#include "render/renderLib.h"

#include <algorithm>

namespace {

const glm::vec3 kLightDir = glm::normalize(glm::vec3(1.0f, 10.0f, 10.0f));

Geometry<TerrainVertex> buildSquare() {
    const glm::vec3 corners[4] = {
        {-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
    Geometry<TerrainVertex> geometry{};
    for (const glm::vec3 &corner : corners)
        geometry.addVertex(
            {corner, glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)});
    geometry.addTriangle(0, 1, 2);
    geometry.addTriangle(0, 2, 3);
    return geometry;
}

} // namespace

GameView::GameView(World &world, Renderer &renderer)
    : renderer(renderer), world(world) {
    camera.setMinMax(0.01f, 400.0f);
    camera.setFOV(glm::radians(90.0f));

    textureManager = std::make_shared<TextureManager>();
    shaders = {
        {"terrain", std::make_shared<Shader>("shaders/shader.vert",
                                             "shaders/shader.frag")},
        {"sun",
         std::make_shared<Shader>("shaders/sun.vert", "shaders/sun.frag")},
    };
    terrainVertexMap = std::make_shared<TerrainVertexMap>(textureManager);

    sun = std::make_unique<Mesh<TerrainVertex>>(
        buildSquare(), textureManager->getTexture("blocks"), shaders["sun"]);

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
    shaders["terrain"]->use();
    shaders["terrain"]->setMat4("projection",
                                camera.getMatrixProjection(ratio));
    shaders["terrain"]->setMat4("view", camera.getMatrixView());

    renderSun(ratio);
    renderChunks(player.chunk);
    firstPersonView->render(player.chunk);
}

void GameView::renderSun(float ratio) {
    glm::vec3 sunPos = camera.position + kLightDir * 300.0f;
    glm::vec3 toCam = glm::normalize(camera.position - sunPos);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), toCam));
    glm::vec3 up = glm::cross(toCam, right);

    float radius = 18.0f;
    glm::mat4 model(1.0f);
    model[0] = glm::vec4(right * radius, 0.0f);
    model[1] = glm::vec4(up * radius, 0.0f);
    model[2] = glm::vec4(toCam, 0.0f);
    model[3] = glm::vec4(sunPos, 1.0f);

    shaders["sun"]->use();
    shaders["sun"]->setMat4("projection", camera.getMatrixProjection(ratio));
    shaders["sun"]->setMat4("view", camera.getMatrixView());
    shaders["sun"]->setMat4("model", model);

    glDepthMask(GL_FALSE);
    sun->render();
    glDepthMask(GL_TRUE);
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
