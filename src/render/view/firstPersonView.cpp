#include "render/view/firstPersonView.h"

#include "world/world.h"

#include <optional>

std::optional<glm::vec3> get_player_looking(Player &player, World &world) {
    glm::vec3 initial_pos =
        player.position + player.size * glm::vec3(0.5f, 0.9f, 0.5f);
    glm::vec3 dir = glm::normalize(player.direction);
    glm::vec3 i = glm::vec3(0);

    while (world.getChunkManager()
               ->getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
               .id == BlockID::air) {
        if (i.x > 6)
            return std::nullopt;
        i += glm::vec3(0.25);
    }
    return std::make_optional<glm::vec3>(initial_pos + dir * i);
}

Mesh<TerrainVertex> createBoxMesh(Player &player, World &world,
                                  TerrainVertexMap &terrainVertexMap,
                                  std::shared_ptr<Texture> texture,
                                  std::shared_ptr<Shader> shader) {

    auto block = Block(BlockID::smooth_stone, false);
    auto blockGeometry = terrainVertexMap.getGeometry(block, {}, ALL_FACE);
    for (auto &vertex : blockGeometry.vertices) {
        vertex.color = glm::vec3(0.3, 0.3, 0.3);
    }
    return Mesh<TerrainVertex>(std::move(blockGeometry), texture, shader);
}

FirstPersonView::FirstPersonView(std::shared_ptr<Player> player,
                                 std::shared_ptr<World> world,
                                 TerrainVertexMap &terrainVertexMap,
                                 std::shared_ptr<Texture> texture,
                                 std::shared_ptr<Shader> shader)
    : player(player), world(world),
      selectionBoxMesh(
          createBoxMesh(*player, *world, terrainVertexMap, texture, shader)) {}

void FirstPersonView::render(int chunkx, int chunkz) {
    selectionBoxMesh.setupMesh();

    if (auto og_pos = get_player_looking(*player, *world); og_pos.has_value()) {

        glm::vec3 pos = floor(og_pos.value());

        selectionBoxMesh.shader->setMat4("model",
                                         glm::translate(glm::mat4(1.0f), pos));

        glLineWidth(5.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        selectionBoxMesh.render();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
