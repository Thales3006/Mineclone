#include "render/view/chunkView.h"

#include "world/terrain/chunk/chunk.h"

ChunkView::ChunkView(ChunkMeshData data, TerrainVertexMap &terrainVertexMap,
                     const std::shared_ptr<TextureManager> &textureManager,
                     std::shared_ptr<Shader> shader)
    : x(data.x), z(data.z), terrainVertexMap(&terrainVertexMap),
      mesh(std::move(data.geometry), textureManager->getTexture("blocks"),
           std::move(shader)),
      blockRanges(std::move(data.blockRanges)) {}

int ChunkView::blockIndex(int i, int j, int k) {
    return (i * CHUNK_HEIGHT + j) * CHUNK_WIDTH + k;
}

Geometry<TerrainVertex> ChunkView::blockGeometry(
    TerrainVertexMap &terrainVertexMap, const RegionSnapshot &region, int i,
    int j, int k) {
    Geometry<TerrainVertex> geometry =
        terrainVertexMap.getGeometry(region, i, j, k);
    geometry.addVertexOffset(glm::vec3(i, j, k));
    return geometry;
}

ChunkMeshData ChunkView::buildMeshData(const RegionSnapshot &region,
                                       TerrainVertexMap &terrainVertexMap) {
    ChunkMeshData data;
    data.x = region.main->x;
    data.z = region.main->z;

    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                if (region.getBlockFaces(i, j, k) == NO_FACE)
                    continue;

                Geometry<TerrainVertex> geometry =
                    blockGeometry(terrainVertexMap, region, i, j, k);
                if (geometry.vertices.empty())
                    continue;

                unsigned int start = data.geometry.vertices.size();
                data.geometry.append(geometry);
                data.blockRanges[blockIndex(i, j, k)] = {
                    start, static_cast<unsigned int>(geometry.vertices.size())};
            }
        }
    }

    return data;
}

void ChunkView::respliceBlock(const RegionSnapshot &region, int i, int j, int k) {
    int index = blockIndex(i, j, k);

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

    if (region.getBlockFaces(i, j, k) == NO_FACE)
        return;

    Geometry<TerrainVertex> geometry =
        blockGeometry(*terrainVertexMap, region, i, j, k);
    if (geometry.vertices.empty())
        return;

    unsigned int start = mesh.getGeometry().vertices.size();
    mesh.append(geometry);
    blockRanges[index] = {start,
                          static_cast<unsigned int>(geometry.vertices.size())};
}

void ChunkView::updateBlock(const RegionSnapshot &region, glm::ivec3 worldEdit) {
    int lx = worldEdit.x - x * CHUNK_WIDTH;
    int ly = worldEdit.y;
    int lz = worldEdit.z - z * CHUNK_WIDTH;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dz = -1; dz <= 1; dz++) {
                int bx = lx + dx;
                int by = ly + dy;
                int bz = lz + dz;

                if (bx < 0 || bx >= CHUNK_WIDTH || by < 0 ||
                    by >= CHUNK_HEIGHT || bz < 0 || bz >= CHUNK_WIDTH)
                    continue;

                respliceBlock(region, bx, by, bz);
            }
        }
    }
}

void ChunkView::render(int chunkx, int chunkz) {
    mesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f),
                                glm::vec3((x - chunkx) * CHUNK_WIDTH, 0,
                                          (z - chunkz) * CHUNK_WIDTH)));
    mesh.render();
}
