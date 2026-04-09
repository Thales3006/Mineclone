#include "render/view/chunkView.h"

#include "world/terrain/chunk/chunk.h"

Mesh<TerrainVertex>
ChunkView::generateChunkMesh(ChunkRegion region,
                             TerrainVertexMap &terrainVertexMap,
                             std::shared_ptr<TextureManager> textureManager,
                             std::shared_ptr<Shader> shader) {

    Geometry<TerrainVertex> chunkGeometry{};
    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                unsigned char faces = region.getBlockFaces(i, j, k);

                if (faces == NO_FACE)
                    continue;

                Block &block = region.main->blocks[i][j][k];
                auto blockGeometry = terrainVertexMap.getGeometry(
                    block, region.getBlockOcclusion(i, j, k), faces);

                glm::vec3 pos = glm::vec3(i, j, k);
                blockGeometry.addVertexOffset(pos);
                chunkGeometry.append(blockGeometry);
            }
        }
    }
    return Mesh<TerrainVertex>(std::move(chunkGeometry),
                               textureManager->getTexture("blocks"), shader);
}

ChunkView::ChunkView(ChunkRegion region, TerrainVertexMap &terrainVertexMap,
                     std::shared_ptr<TextureManager> textureManager,
                     std::shared_ptr<Shader> shader)
    : region(region), mesh(generateChunkMesh(region, terrainVertexMap,
                                             textureManager, shader)) {}

void ChunkView::render(int chunkx, int chunkz) {
    mesh.setupMesh();
    mesh.shader->setMat4(
        "model",
        glm::translate(glm::mat4(1.0f),
                       glm::vec3((region.main->x - chunkx) * CHUNK_WIDTH, 0,
                                 (region.main->z - chunkz) * CHUNK_WIDTH)));
    mesh.render();
}
