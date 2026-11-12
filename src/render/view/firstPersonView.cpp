#include "render/view/firstPersonView.h"

#include "world/terrain/raycast.h"
#include "world/world.h"

Mesh<TerrainVertex> createBoxMesh(Player &player, World &world,
                                  TerrainVertexMap &terrainVertexMap,
                                  std::shared_ptr<Texture> texture,
                                  std::shared_ptr<Shader> shader) {

    auto block = Block(BlockID::smooth_stone, false);
    auto blockGeometry = terrainVertexMap.getSelectionBox();
    for (auto &vertex : blockGeometry.vertices) {
        vertex.color = glm::vec3(0.3, 0.3, 0.3);
    }
    return Mesh<TerrainVertex>(std::move(blockGeometry), texture, shader);
}

FirstPersonView::FirstPersonView(std::shared_ptr<Player> player, World &world,
                                 TerrainVertexMap &terrainVertexMap,
                                 std::shared_ptr<Texture> texture,
                                 std::shared_ptr<Shader> shader)
    : player(player), world(world),
      selectionBoxMesh(
          createBoxMesh(*player, world, terrainVertexMap, texture, shader)) {}

void FirstPersonView::render(int chunkx, int chunkz) {
    auto hit = raycastVoxel(world.getChunkManager(), player->chunkx,
                            player->chunkz, player->eyePosition(),
                            player->direction, 6.0f);
    if (!hit)
        return;

    selectionBoxMesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f), glm::vec3(hit->block)));

    glLineWidth(5.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    selectionBoxMesh.render();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
}
