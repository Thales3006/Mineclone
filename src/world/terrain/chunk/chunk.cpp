#include "world/terrain/chunk/chunk.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FastNoiseLite.h>

Chunk::Chunk() : Chunk(0, 0) {}

Chunk::Chunk(int x, int z) : x(x), z(z) {}

Chunk::Chunk(int x, int z, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH])
    : Chunk(x, z) {

    for (int h = 0; h < CHUNK_WIDTH; ++h) {
        for (int w = 0; w < CHUNK_HEIGHT; ++w) {
            for (int d = 0; d < CHUNK_WIDTH; ++d) {
                this->blocks[h][w][d] = blocks[h][w][d];
            }
        }
    }
}

Chunk::~Chunk() {}

std::shared_ptr<Chunk> Chunk::generateChunk(int x, int z) {
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(x, z);

    FastNoiseLite noise;
    noise.SetSeed(42);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.05f);

    FastNoiseLite cave_noise;
    cave_noise.SetSeed(42);
    cave_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    cave_noise.SetFrequency(0.1f);

    for (int i = CHUNK_WIDTH - 1; i >= 0; i--) {
        for (int j = CHUNK_HEIGHT - 1; j >= 0; j--) {
            for (int k = CHUNK_WIDTH - 1; k >= 0; k--) {

                float val =
                    5 * noise.GetNoise(static_cast<float>(x * CHUNK_WIDTH + i),
                                       static_cast<float>(z * CHUNK_WIDTH + k));

                float cave_val = cave_noise.GetNoise(
                    static_cast<float>(x * CHUNK_WIDTH + i),
                    static_cast<float>(j),
                    static_cast<float>(z * CHUNK_WIDTH + k));

                Block block{};
                if ((j < static_cast<float>(CHUNK_HEIGHT) / 3 + val) &&
                    (cave_val < 0.2)) {
                    BlockID id = BlockID::stone;
                    // stone or dirt
                    if (j > CHUNK_HEIGHT / 4) {
                        // grass or dirt
                        if (j == CHUNK_WIDTH - 1 ||
                            chunk->blocks[i][j + 1][k].id == BlockID::air) {
                            id = BlockID::grass_block;
                            if (cave_val < -0.4) {
                                chunk->blocks[i][j + 1][k] =
                                    Block(BlockID::grass, false);
                            }
                        } else {
                            id = BlockID::dirt;
                        }
                    }
                    block = Block(id, true);
                }
                chunk->blocks[i][j][k] = block;
            }
        }
    }
    return chunk;
}

void Chunk::setBlock(int x, int y, int z, Block block) {
    blocks[x][y][z] = block;
}

void Chunk::updateBlock(int x, int y, int z, const Chunk *leftChunk,
                        const Chunk *rightChunk, const Chunk *frontChunk,
                        const Chunk *backChunk) {
    if (blocks[x][y][z].id == BlockID::air) {
        blocks[x][y][z].faces = NO_FACE;
        return;
    }

    unsigned char newFaces = ALL_FACE;
    if (x != CHUNK_WIDTH - 1 ? blocks[x + 1][y][z].solid != false
        : rightChunk         ? rightChunk->blocks[0][y][z].solid != false
                             : false) {
        newFaces &= ~RIGHT_FACE;
    }
    if (x != 0      ? blocks[x - 1][y][z].solid != false
        : leftChunk ? leftChunk->blocks[CHUNK_WIDTH - 1][y][z].solid != false
                    : false) {
        newFaces &= ~LEFT_FACE;
    }
    if (z != CHUNK_WIDTH - 1 ? blocks[x][y][z + 1].solid != false
        : frontChunk         ? frontChunk->blocks[x][y][0].solid != false
                             : false) {
        newFaces &= ~FRONT_FACE;
    }
    if (z != 0      ? blocks[x][y][z - 1].solid != false
        : backChunk ? backChunk->blocks[x][y][CHUNK_WIDTH - 1].solid != false
                    : false) {
        newFaces &= ~BACK_FACE;
    }
    if (y != CHUNK_HEIGHT - 1 && blocks[x][y + 1][z].solid != false) {
        newFaces &= ~UP_FACE;
    }
    if (y != 0 && blocks[x][y - 1][z].solid != false) {
        newFaces &= ~DOWN_FACE;
    }

    blocks[x][y][z].faces = newFaces;
}

void Chunk::updateBlocks(const Chunk *leftChunk, const Chunk *rightChunk,
                         const Chunk *frontChunk, const Chunk *backChunk) {

    for (int x = 0; x < CHUNK_WIDTH; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_WIDTH; z++) {
                updateBlock(x, y, z, leftChunk, rightChunk, frontChunk,
                            backChunk);
            }
}

void Chunk::updateSide(const Side side, const Chunk *leftChunk,
                       const Chunk *rightChunk, const Chunk *frontChunk,
                       const Chunk *backChunk) {
    bool isXVar = (side == front || side == back)
                      ? true
                      : false; // checks if X or Z is the variable

    int x = (side == right) ? CHUNK_WIDTH - 1 : 0;
    int z = (side == front) ? CHUNK_WIDTH - 1 : 0;

    while (x < CHUNK_WIDTH && z < CHUNK_WIDTH) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            updateBlock(x, y, z, leftChunk, rightChunk, frontChunk, backChunk);
        }
        if (isXVar) {
            x++;
        } else {
            z++;
        }
    }
}

glm::vec3 Chunk::chunkOffSet(glm::vec3 position) {
    int offsetX = static_cast<int>(
        std::floor(position.x / static_cast<float>(CHUNK_WIDTH)));
    int offsetZ = static_cast<int>(
        std::floor(position.z / static_cast<float>(CHUNK_WIDTH)));

    return glm::vec3(offsetX, 0, offsetZ);
}
