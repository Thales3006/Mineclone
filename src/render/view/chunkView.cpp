#include "render/view/chunkView.h"

#include "world/terrain/chunk/chunk.h"

Mesh<TerrainVertex>
generateChunkMesh(Chunk &chunk, std::vector<std::shared_ptr<Texture>> textures,
                  std::shared_ptr<Shader> shader) {
    std::vector<TerrainVertex> drawableFaces;
    std::vector<unsigned int> indices;

    unsigned int count = 0;
    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                Block &block = chunk.blocks[i][j][k];
                if (block.ID == 0 || block.faces == NO_FACE)
                    continue;

                glm::vec3 pos = glm::vec3(i, j, k);
                //{0,3,2, 0,2,1}
                if (block.faces & FRONT_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(3 + count * 4);
                    count++;
                }
                if (block.faces & BACK_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, -1.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(0 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(1 + count * 4);
                    count++;
                }
                if (block.faces & RIGHT_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                      glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                      glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                                      glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(3 + count * 4);
                    count++;
                }
                if (block.faces & LEFT_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(-1.0f, 0.0f, 0.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec3(-1.0f, 0.0f, 0.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                                      glm::vec3(-1.0f, 0.0f, 0.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec3(-1.0f, 0.0f, 0.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(0 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(1 + count * 4);
                    count++;
                }
                if (block.faces & UP_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(1 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(3 + count * 4);
                    count++;
                }
                if (block.faces & DOWN_FACE) {
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f),
                                      glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f),
                                      glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f),
                                      glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        TerrainVertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, -1.0f, 0.0f),
                                      glm::vec2(1.0f, 0.0f), float(block.ID)});
                    indices.push_back(0 + count * 4);
                    indices.push_back(3 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(0 + count * 4);
                    indices.push_back(2 + count * 4);
                    indices.push_back(1 + count * 4);
                    count++;
                }
            }
        }
    }
    return Mesh<TerrainVertex>(drawableFaces, indices, textures, shader);
}

ChunkView::ChunkView(std::shared_ptr<Chunk> chunk,
                     std::vector<std::shared_ptr<Texture>> textures,
                     std::shared_ptr<Shader> shader)
    : chunk(chunk), mesh(generateChunkMesh(*chunk, textures, shader)) {}

void ChunkView::render(int chunkx, int chunkz) {
    auto chunkPtr = chunk.lock();
    if (!chunkPtr) {
        return;
    }
    mesh.shader->setMat4(
        "model",
        glm::translate(glm::mat4(1.0f),
                       glm::vec3((chunkPtr->x - chunkx) * CHUNK_WIDTH, 0,
                                 (chunkPtr->z - chunkz) * CHUNK_WIDTH)));
    mesh.render();
}
