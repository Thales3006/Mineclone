#include "render/view/chunkView.h"

#include "world/terrain/chunk/chunk.h"

Mesh<TerrainVertex>
generateChunkMesh(Chunk &chunk, TerrainVertexMap &terrainVertexMap,
                  std::shared_ptr<TextureManager> textureManager,
                  std::shared_ptr<Shader> shader) {

    Geometry<TerrainVertex> chunkGeometry{};
    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                Block &block = chunk.blocks[i][j][k];
                if (block.id == BlockID::air || block.faces == NO_FACE)
                    continue;

                glm::vec3 pos = glm::vec3(i, j, k);

                auto blockGeometry = terrainVertexMap.getGeometry(block);
                blockGeometry.addVertexOffset(pos);
                chunkGeometry.append(blockGeometry);
            }
        }
    }
    return Mesh<TerrainVertex>(std::move(chunkGeometry),
                               textureManager->getTexture("blocks"), shader);
}

ChunkView::ChunkView(std::shared_ptr<Chunk> chunk,
                     TerrainVertexMap &terrainVertexMap,
                     std::shared_ptr<TextureManager> textureManager,
                     std::shared_ptr<Shader> shader)
    : chunk(chunk), mesh(generateChunkMesh(*chunk, terrainVertexMap,
                                           textureManager, shader)) {}

void ChunkView::render(int chunkx, int chunkz) {
    mesh.setupMesh();
    mesh.shader->setMat4(
        "model", glm::translate(glm::mat4(1.0f),
                                glm::vec3((chunk->x - chunkx) * CHUNK_WIDTH, 0,
                                          (chunk->z - chunkz) * CHUNK_WIDTH)));
    mesh.render();
}
