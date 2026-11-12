#include "render/view/firstPersonView.h"

#include "world/terrain/block/blockRegistry.h"
#include "world/terrain/raycast.h"
#include "world/world.h"

namespace {

const unsigned int kFaceEdges[6][8] = {
    {1, 2, 2, 6, 6, 5, 5, 1}, // +X
    {0, 3, 3, 7, 7, 4, 4, 0}, // -X
    {4, 5, 5, 6, 6, 7, 7, 4}, // +Y
    {0, 1, 1, 2, 2, 3, 3, 0}, // -Y
    {3, 2, 2, 6, 6, 7, 7, 3}, // +Z
    {0, 1, 1, 5, 5, 4, 4, 0}, // -Z
};

Geometry<TerrainVertex> buildOutline(const AABB &box, glm::vec3 localCamera,
                                     glm::vec2 uv) {
    const glm::vec3 &lo = box.min;
    const glm::vec3 &hi = box.max;
    const glm::vec3 corners[8] = {
        {lo.x, lo.y, lo.z}, {hi.x, lo.y, lo.z}, {hi.x, lo.y, hi.z},
        {lo.x, lo.y, hi.z}, {lo.x, hi.y, lo.z}, {hi.x, hi.y, lo.z},
        {hi.x, hi.y, hi.z}, {lo.x, hi.y, hi.z},
    };

    Geometry<TerrainVertex> geometry{};
    for (const glm::vec3 &corner : corners)
        geometry.addVertex({corner, glm::vec3(0.0f), uv, glm::vec3(0.0f)});

    auto addFace = [&geometry](int face) {
        for (int i = 0; i < 8; i++)
            geometry.indices.push_back(kFaceEdges[face][i]);
    };

    if (localCamera.x > hi.x)
        addFace(0);
    else if (localCamera.x < lo.x)
        addFace(1);

    if (localCamera.y > hi.y)
        addFace(2);
    else if (localCamera.y < lo.y)
        addFace(3);

    if (localCamera.z > hi.z)
        addFace(4);
    else if (localCamera.z < lo.z)
        addFace(5);

    return geometry;
}

} // namespace

Mesh<TerrainVertex> createBoxMesh(TerrainVertexMap &terrainVertexMap,
                                  std::shared_ptr<Texture> texture,
                                  std::shared_ptr<Shader> shader) {
    Mesh<TerrainVertex> mesh(terrainVertexMap.getSelectionBox(),
                             std::move(texture), std::move(shader));
    mesh.setDrawMode(GL_LINES);
    return mesh;
}

FirstPersonView::FirstPersonView(std::shared_ptr<Player> player, World &world,
                                 TerrainVertexMap &terrainVertexMap,
                                 std::shared_ptr<Texture> texture,
                                 std::shared_ptr<Shader> shader)
    : selectionBoxMesh(createBoxMesh(terrainVertexMap, std::move(texture),
                                     std::move(shader))),
      selectionUV(terrainVertexMap.getSelectionBox().vertices[0].uv),
      player(std::move(player)), world(world) {}

void FirstPersonView::render(glm::ivec2 chunk) {
    auto hit = raycastVoxel(world.getChunkManager(), player->chunk,
                            player->eyePosition(), player->direction, 6.0f);
    if (!hit)
        return;

    glm::vec3 localCamera = player->eyePosition() - glm::vec3(hit->block);

    Geometry<TerrainVertex> outline{};
    for (const AABB &box : blockDef(hit->id).selection)
        outline.append(buildOutline(box, localCamera, selectionUV));
    selectionBoxMesh.setGeometry(outline);

    selectionBoxMesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f), glm::vec3(hit->block)));

    glLineWidth(1.0f);
    glDepthFunc(GL_LEQUAL);
    selectionBoxMesh.shader->setFloat("depthBias", 0.01f);
    selectionBoxMesh.render();
    selectionBoxMesh.shader->setFloat("depthBias", 0.0f);
    glDepthFunc(GL_LESS);
}
