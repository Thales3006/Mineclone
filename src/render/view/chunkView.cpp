#include "render/view/chunkView.h"

#include "world/terrain/chunk/chunk.h"

ChunkView::ChunkView(ChunkMeshData data, TerrainVertexMap &terrainVertexMap,
                     const std::shared_ptr<TextureManager> &textureManager,
                     std::shared_ptr<Shader> shader)
    : coord(data.coord), terrainVertexMap(&terrainVertexMap),
      mesh(std::move(data.geometry), textureManager->getTexture("blocks"),
           std::move(shader)),
      blockRanges(std::move(data.blockRanges)) {}

int ChunkView::blockIndex(glm::ivec3 pos) {
    return (pos.x * CHUNK_HEIGHT + pos.y) * CHUNK_WIDTH + pos.z;
}

Geometry<TerrainVertex> ChunkView::blockGeometry(
    TerrainVertexMap &terrainVertexMap, const RegionSnapshot &region,
    glm::ivec3 pos) {
    Geometry<TerrainVertex> geometry = terrainVertexMap.getGeometry(region, pos);
    geometry.addVertexOffset(glm::vec3(pos));
    return geometry;
}

ChunkMeshData ChunkView::buildMeshData(const RegionSnapshot &region,
                                       TerrainVertexMap &terrainVertexMap) {
    ChunkMeshData data;
    data.coord = region.main->coord;

    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                glm::ivec3 pos(i, j, k);
                if (region.getBlockFaces(pos) == NO_FACE)
                    continue;

                Geometry<TerrainVertex> geometry =
                    blockGeometry(terrainVertexMap, region, pos);
                if (geometry.vertices.empty())
                    continue;

                unsigned int start = data.geometry.vertices.size();
                data.geometry.append(geometry);
                data.blockRanges[blockIndex(pos)] = {
                    start, static_cast<unsigned int>(geometry.vertices.size())};
            }
        }
    }

    return data;
}

void ChunkView::respliceBlock(const RegionSnapshot &region, glm::ivec3 pos) {
    int index = blockIndex(pos);

    auto it = blockRanges.find(index);
    if (it != blockRanges.end()) {
        unsigned int start = it->second.first;
        unsigned int count = it->second.second;

        mesh.removeVertices(start, count);
        blockRanges.erase(it);

        for (auto &entry : blockRanges)
            if (entry.second.first > start)
                entry.second.first -= count;
    }

    if (region.getBlockFaces(pos) == NO_FACE)
        return;

    Geometry<TerrainVertex> geometry =
        blockGeometry(*terrainVertexMap, region, pos);
    if (geometry.vertices.empty())
        return;

    unsigned int start = mesh.getGeometry().vertices.size();
    mesh.append(geometry);
    blockRanges[index] = {start,
                          static_cast<unsigned int>(geometry.vertices.size())};
}

void ChunkView::updateBlock(const RegionSnapshot &region, glm::ivec3 worldEdit) {
    glm::ivec3 local(worldEdit.x - coord.x * CHUNK_WIDTH, worldEdit.y,
                     worldEdit.z - coord.y * CHUNK_WIDTH);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                glm::ivec3 pos = local + glm::ivec3(dx, dy, dz);

                if (pos.x < 0 || pos.x >= CHUNK_WIDTH || pos.y < 0 ||
                    pos.y >= CHUNK_HEIGHT || pos.z < 0 || pos.z >= CHUNK_WIDTH)
                    continue;

                respliceBlock(region, pos);
            }
        }
    }
}

void ChunkView::render(glm::ivec2 chunk) {
    mesh.shader->use();
    mesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f),
                                glm::vec3((coord.x - chunk.x) * CHUNK_WIDTH, 0,
                                          (coord.y - chunk.y) * CHUNK_WIDTH)));
    mesh.render();
}
