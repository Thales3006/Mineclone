#include "render/view/firstPersonView.h"

#include "world/terrain/raycast.h"
#include "world/world.h"

namespace {

const glm::vec3 kCorners[8] = {
    {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
    {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1},
};

const unsigned int kFaceEdges[6][8] = {
    {1, 2, 2, 6, 6, 5, 5, 1}, // +X
    {0, 3, 3, 7, 7, 4, 4, 0}, // -X
    {4, 5, 5, 6, 6, 7, 7, 4}, // +Y
    {0, 1, 1, 2, 2, 3, 3, 0}, // -Y
    {3, 2, 2, 6, 6, 7, 7, 3}, // +Z
    {0, 1, 1, 5, 5, 4, 4, 0}, // -Z
};

Geometry<TerrainVertex> buildOutline(glm::vec3 localCamera, glm::vec2 uv) {
    Geometry<TerrainVertex> geometry{};
    for (const glm::vec3 &corner : kCorners)
        geometry.addVertex({corner, glm::vec3(0.0f), uv, glm::vec3(0.0f)});

    auto addFace = [&geometry](int face) {
        for (int i = 0; i < 8; i++)
            geometry.indices.push_back(kFaceEdges[face][i]);
    };

    if (localCamera.x > 1.0f)
        addFace(0);
    else if (localCamera.x < 0.0f)
        addFace(1);

    if (localCamera.y > 1.0f)
        addFace(2);
    else if (localCamera.y < 0.0f)
        addFace(3);

    if (localCamera.z > 1.0f)
        addFace(4);
    else if (localCamera.z < 0.0f)
        addFace(5);

    return geometry;
}

} // namespace

Mesh<TerrainVertex> createBoxMesh(TerrainVertexMap &terrainVertexMap,
                                  std::shared_ptr<Texture> texture,
                                  std::shared_ptr<Shader> shader) {
    Mesh<TerrainVertex> mesh(terrainVertexMap.getSelectionBox(), texture,
                             shader);
    mesh.setDrawMode(GL_LINES);
    return mesh;
}

FirstPersonView::FirstPersonView(std::shared_ptr<Player> player, World &world,
                                 TerrainVertexMap &terrainVertexMap,
                                 std::shared_ptr<Texture> texture,
                                 std::shared_ptr<Shader> shader)
    : selectionBoxMesh(createBoxMesh(terrainVertexMap, texture, shader)),
      selectionUV(terrainVertexMap.getSelectionBox().vertices[0].uv),
      player(player), world(world) {}

void FirstPersonView::render(int chunkx, int chunkz) {
    auto hit = raycastVoxel(world.getChunkManager(), player->chunkx,
                            player->chunkz, player->eyePosition(),
                            player->direction, 6.0f);
    if (!hit)
        return;

    glm::vec3 localCamera = player->eyePosition() - glm::vec3(hit->block);
    selectionBoxMesh.setGeometry(buildOutline(localCamera, selectionUV));

    selectionBoxMesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f), glm::vec3(hit->block)));

    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    selectionBoxMesh.render();
    glLineWidth(1.0f);
    glEnable(GL_DEPTH_TEST);
}
