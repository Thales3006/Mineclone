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
               .ID == 0) {
        if (i.x > 6)
            return std::nullopt;
        i += glm::vec3(0.25);
    }
    return std::make_optional<glm::vec3>(initial_pos + dir * i);
}

Mesh<TerrainVertex> createBoxMesh(Player &player, World &world,
                                  std::shared_ptr<Texture> texture,
                                  std::shared_ptr<Shader> shader) {
    std::vector<TerrainVertex> drawableFaces;
    std::vector<unsigned int> indices;

    drawableFaces.insert(drawableFaces.end(),
                         {
                             TerrainVertex{glm::vec3(-0.001f, -0.001f, -0.001f),
                                           glm::vec3(-1.0f, -1.0f, -1.0f),
                                           glm::vec2(0.0f, 0.0f), 14}, // 0
                             TerrainVertex{glm::vec3(1.001f, -0.001f, -0.001f),
                                           glm::vec3(1.0f, -1.0f, -1.0f),
                                           glm::vec2(1.0f, 0.0f), 14}, // 1
                             TerrainVertex{glm::vec3(1.001f, 1.001f, -0.001f),
                                           glm::vec3(1.0f, 1.0f, -1.0f),
                                           glm::vec2(1.0f, 1.0f), 14}, // 2
                             TerrainVertex{glm::vec3(-0.001f, 1.001f, -0.001f),
                                           glm::vec3(-1.0f, 1.0f, -1.0f),
                                           glm::vec2(0.0f, 1.0f), 14}, // 3
                             TerrainVertex{glm::vec3(-0.001f, -0.001f, 1.001f),
                                           glm::vec3(-1.0f, -1.0f, 1.0f),
                                           glm::vec2(0.0f, 0.0f), 14}, // 4
                             TerrainVertex{glm::vec3(1.001f, -0.001f, 1.001f),
                                           glm::vec3(1.0f, -1.0f, 1.0f),
                                           glm::vec2(1.0f, 0.0f), 14}, // 5
                             TerrainVertex{glm::vec3(1.001f, 1.001f, 1.001f),
                                           glm::vec3(1.0f, 1.0f, 1.0f),
                                           glm::vec2(1.0f, 1.0f), 14}, // 6
                             TerrainVertex{glm::vec3(-0.001f, 1.001f, 1.001f),
                                           glm::vec3(-1.0f, 1.0f, 1.0f),
                                           glm::vec2(0.0f, 1.0f), 14}, // 7
                         });

    indices.insert(indices.end(), {
                                      0, 2, 1, 0, 3, 2, // frente
                                      4, 5, 6, 4, 6, 7, // trás
                                      0, 4, 7, 0, 7, 3, // esquerda
                                      1, 6, 5, 1, 2, 6, // direita
                                      0, 5, 4, 0, 1, 5, // baixo
                                      3, 6, 2, 3, 7, 6, // cima
                                  });

    return Mesh<TerrainVertex>(drawableFaces, indices, texture, shader);
}

FirstPersonView::FirstPersonView(std::shared_ptr<Player> player,
                                 std::shared_ptr<World> world,
                                 std::shared_ptr<Texture> texture,
                                 std::shared_ptr<Shader> shader)
    : player(player), world(world),
      selectionBoxMesh(createBoxMesh(*player, *world, texture, shader)) {}

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
