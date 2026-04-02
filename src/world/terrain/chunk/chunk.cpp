#include "world/terrain/chunk/chunk.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Chunk::Chunk() {
    x = 0;
    z = 0;
}

Chunk::Chunk(int x, int z) {
    this->x = x;
    this->z = z;
}

Chunk::Chunk(int x, int z, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]) {
    this->x = x;
    this->z = z;

    for (int h = 0; h < CHUNK_WIDTH; ++h) {
        for (int w = 0; w < CHUNK_HEIGHT; ++w) {
            for (int d = 0; d < CHUNK_WIDTH; ++d) {
                this->blocks[h][w][d] = blocks[h][w][d];
            }
        }
    }
}

Chunk::~Chunk() {}

std::unique_ptr<Chunk> Chunk::generateChunk(int x, int z) {
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(x, z);
    for (int i = CHUNK_WIDTH - 1; i >= 0; i--) {
        for (int j = CHUNK_HEIGHT - 1; j >= 0; j--) {
            for (int k = CHUNK_WIDTH - 1; k >= 0; k--) {
                int f = 2 * (cos((i + k * sin(x + z)) / 4 *
                                 cos(static_cast<float>((z + x) * CHUNK_WIDTH + k + j) / 10)) +
                             cos((k - i * sin(z + x)) / 3));
                chunk->blocks[i][j][k] =
                    j < CHUNK_HEIGHT / 3 + f
                        ? Block(j < CHUNK_HEIGHT / 3                                           ? 1
                                : (j == CHUNK_WIDTH - 1 || chunk->blocks[i][j + 1][k].ID == 0) ? 3
                                                                                               : 2,
                                true)
                        : Block();
            }
        }
    }
    return chunk;
}

void Chunk::setBlock(int x, int y, int z, Block block) { blocks[x][y][z] = block; }

void Chunk::updateBlock(int x, int y, int z, const Chunk *leftChunk, const Chunk *rightChunk,
                        const Chunk *frontChunk, const Chunk *backChunk) {
    if (blocks[x][y][z].ID == 0) {
        blocks[x][y][z].faces = 0;
        return;
    }

    unsigned char newFaces = ALL_FACE;
    if (x != CHUNK_WIDTH - 1 ? blocks[x + 1][y][z].ID != 0
        : rightChunk         ? rightChunk->blocks[0][y][z].ID != 0
                             : false)
        newFaces &= ~RIGHT_FACE;
    if (x != 0      ? blocks[x - 1][y][z].ID != 0
        : leftChunk ? leftChunk->blocks[CHUNK_WIDTH - 1][y][z].ID != 0
                    : false)
        newFaces &= ~LEFT_FACE;
    if (z != CHUNK_WIDTH - 1 ? blocks[x][y][z + 1].ID != 0
        : frontChunk         ? frontChunk->blocks[x][y][0].ID != 0
                             : false)
        newFaces &= ~FRONT_FACE;
    if (z != 0      ? blocks[x][y][z - 1].ID != 0
        : backChunk ? backChunk->blocks[x][y][CHUNK_WIDTH - 1].ID != 0
                    : false)
        newFaces &= ~BACK_FACE;
    if (y != CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].ID != 0)
        newFaces &= ~UP_FACE;
    if (y != 0 && blocks[x][y - 1][z].ID != 0)
        newFaces &= ~DOWN_FACE;

    blocks[x][y][z].faces = newFaces;
}

void Chunk::updateBlocks(const Chunk *leftChunk, const Chunk *rightChunk, const Chunk *frontChunk,
                         const Chunk *backChunk) {

    for (int x = 0; x < CHUNK_WIDTH; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_WIDTH; z++) {
                if (blocks[x][y][z].ID == 0) {
                    blocks[x][y][z].faces = 0;
                    continue;
                }
                unsigned char newFaces = ALL_FACE;

                if (x != CHUNK_WIDTH - 1 ? blocks[x + 1][y][z].ID != 0
                    : rightChunk         ? rightChunk->blocks[0][y][z].ID != 0
                                         : false)
                    newFaces &= ~RIGHT_FACE;
                if (x != 0      ? blocks[x - 1][y][z].ID != 0
                    : leftChunk ? leftChunk->blocks[CHUNK_WIDTH - 1][y][z].ID != 0
                                : false)
                    newFaces &= ~LEFT_FACE;

                if (z != CHUNK_WIDTH - 1 ? blocks[x][y][z + 1].ID != 0
                    : frontChunk         ? frontChunk->blocks[x][y][0].ID != 0
                                         : false)
                    newFaces &= ~FRONT_FACE;
                if (z != 0      ? blocks[x][y][z - 1].ID != 0
                    : backChunk ? backChunk->blocks[x][y][CHUNK_WIDTH - 1].ID != 0
                                : false)
                    newFaces &= ~BACK_FACE;

                if (y != CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].ID != 0)
                    newFaces &= ~UP_FACE;
                if (y != 0 && blocks[x][y - 1][z].ID != 0)
                    newFaces &= ~DOWN_FACE;

                blocks[x][y][z].faces = newFaces;
            }
    setMesh();
}

void Chunk::updateSide(const Side side, const Chunk *leftChunk, const Chunk *rightChunk,
                       const Chunk *frontChunk, const Chunk *backChunk) {
    bool isXVar =
        (side == front || side == back) ? true : false; // checks if X or Z is the variable

    int x = (side == right) ? CHUNK_WIDTH - 1 : 0;
    int z = (side == front) ? CHUNK_WIDTH - 1 : 0;

    while (x < CHUNK_WIDTH && z < CHUNK_WIDTH) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {

            if (blocks[x][y][z].ID == 0) {
                blocks[x][y][z].faces = 0;
                continue;
            }
            unsigned char newFaces = ALL_FACE;

            if (x != CHUNK_WIDTH - 1 ? blocks[x + 1][y][z].ID != 0
                : rightChunk         ? rightChunk->blocks[0][y][z].ID != 0
                                     : false)
                newFaces &= ~RIGHT_FACE;
            if (x != 0      ? blocks[x - 1][y][z].ID != 0
                : leftChunk ? leftChunk->blocks[CHUNK_WIDTH - 1][y][z].ID != 0
                            : false)
                newFaces &= ~LEFT_FACE;

            if (z != CHUNK_WIDTH - 1 ? blocks[x][y][z + 1].ID != 0
                : frontChunk         ? frontChunk->blocks[x][y][0].ID != 0
                                     : false)
                newFaces &= ~FRONT_FACE;
            if (z != 0      ? blocks[x][y][z - 1].ID != 0
                : backChunk ? backChunk->blocks[x][y][CHUNK_WIDTH - 1].ID != 0
                            : false)
                newFaces &= ~BACK_FACE;

            if (y != CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].ID != 0)
                newFaces &= ~UP_FACE;
            if (y != 0 && blocks[x][y - 1][z].ID != 0)
                newFaces &= ~DOWN_FACE;

            blocks[x][y][z].faces = newFaces;
        }
        if (isXVar)
            x++;
        else
            z++;
    }
    setMesh();
}

void Chunk::setMesh() {

    std::vector<Vertex> drawableFaces;
    std::vector<unsigned int> indices;

    unsigned int count = 0;
    for (int i = 0; i < CHUNK_WIDTH; i++)
        for (int j = 0; j < CHUNK_HEIGHT; j++)
            for (int k = 0; k < CHUNK_WIDTH; k++) {
                Block &block = blocks[i][j][k];
                if (block.ID == 0 || block.faces == NO_FACE)
                    continue;

                glm::vec3 pos = glm::vec3(i, j, k);
                //{0,3,2, 0,2,1}
                if (block.faces & FRONT_FACE) {
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                                   glm::vec3(0.0f, 0.0f, 1.0f),
                                                   glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                                                   glm::vec3(0.0f, 0.0f, 1.0f),
                                                   glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                                   glm::vec3(0.0f, 0.0f, 1.0f),
                                                   glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
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
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                                                   glm::vec3(0.0f, 0.0f, -1.0f),
                                                   glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec3(0.0f, 0.0f, -1.0f),
                                                   glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                                   glm::vec3(0.0f, 0.0f, -1.0f),
                                                   glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
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
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
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
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
                                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                                   glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
                                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                                   glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
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
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 1.0f),
                                                   glm::vec3(0.0f, 1.0f, 0.0f),
                                                   glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 1.0f),
                                                   glm::vec3(0.0f, 1.0f, 0.0f),
                                                   glm::vec2(1.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 1.0f, 0.0f),
                                                   glm::vec3(0.0f, 1.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 1.0f, 0.0f),
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
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 1.0f),
                                                   glm::vec3(0.0f, -1.0f, 0.0f),
                                                   glm::vec2(0.0f, 0.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 1.0f),
                                                   glm::vec3(0.0f, -1.0f, 0.0f),
                                                   glm::vec2(0.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(1.0f, 0.0f, 0.0f),
                                                   glm::vec3(0.0f, -1.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f), float(block.ID)});
                    drawableFaces.push_back(Vertex{pos + glm::vec3(0.0f, 0.0f, 0.0f),
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

    mesh = std::make_unique<Mesh>(drawableFaces, indices);
}

void Chunk::renderChunk(Shader &shader, int chunkx, int chunkz, std::vector<Texture> textures) {

    shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3((x - chunkx) * CHUNK_WIDTH, 0,
                                                                      (z - chunkz) * CHUNK_WIDTH)));
    if (!mesh->isSetup) {
        mesh->textures = {textures[1]};
        mesh->setupMesh();
    }
    mesh->draw(shader);
}

glm::vec3 Chunk::chunkOffSet(glm::vec3 position) {
    int offsetX = static_cast<int>(std::floor(position.x / static_cast<float>(CHUNK_WIDTH)));
    int offsetZ = static_cast<int>(std::floor(position.z / static_cast<float>(CHUNK_WIDTH)));

    return glm::vec3(offsetX, 0, offsetZ);
}
