#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "world/terrain/block/block.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <memory>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 64

enum Side { front, back, left, right };

class Chunk {
  public:
    glm::ivec2 coord;
    Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

    Chunk();
    Chunk(glm::ivec2 coord);
    Chunk(glm::ivec2 coord, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]);

    ~Chunk();

    void setBlock(glm::ivec3 pos, Block block);

    static std::shared_ptr<Chunk> generateChunk(int seed, glm::ivec2 coord);

    static glm::vec3 chunkOffSet(glm::vec3 position);
};

#endif
