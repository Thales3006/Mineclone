#include "render/mesh/chunkMesh.h"

#include "world/terrain/chunk/chunk.h"

ChunkMesh::ChunkMesh(std::vector<Vertex> vertices,
                     std::vector<unsigned int> indices,
                     std::vector<std::shared_ptr<Texture>> textures,
                     std::shared_ptr<Shader> shader, int x, int z)
    : Mesh(vertices, indices, textures, shader), x(x), z(z) {}

void ChunkMesh::render(int chunkx, int chunkz) {
    shader->setMat4(
        "model",
        glm::translate(glm::mat4(1.0f), glm::vec3((x - chunkx) * CHUNK_WIDTH, 0,
                                                  (z - chunkz) * CHUNK_WIDTH)));
    draw();
}

std::unique_ptr<ChunkMesh>
ChunkMesh::fromChunk(Chunk &chunk,
                     std::vector<std::shared_ptr<Texture>> textures,
                     std::shared_ptr<Shader> shader) {

    std::vector<Vertex> drawableFaces;
    std::vector<unsigned int> indices;

    unsigned int count = 0;
    for (int i = 0; i < CHUNK_WIDTH; i++)
        for (int j = 0; j < CHUNK_HEIGHT; j++)
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                Block &block = chunk.blocks[i][j][k];
                if (block.ID == 0 || block.faces == NO_FACE)
                    continue;

                glm::vec3 pos = glm::vec3(i, j, k);
                //{0,3,2, 0,2,1}
                if (block.faces & FRONT_FACE) {
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
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
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
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
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                               glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
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
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                               glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
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
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
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
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(
                        Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
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

    return std::make_unique<ChunkMesh>(drawableFaces, indices, textures, shader,
                                       chunk.x, chunk.z);
}
