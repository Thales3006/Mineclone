#ifndef CHUNK_REGION_CLASS_H
#define CHUNK_REGION_CLASS_H

#include "world/terrain/chunk/chunk.h"

#include <memory>
#include <optional>

#define FRONT_FACE 0b00000001
#define BACK_FACE 0b00000010
#define LEFT_FACE 0b00000100
#define RIGHT_FACE 0b00001000
#define UP_FACE 0b00010000
#define DOWN_FACE 0b00100000

#define ALL_FACE                                                               \
    RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE
#define NO_FACE 0b00000000

enum Node {
    frontRightUp,
    frontLeftUp,
    backRightUp,
    backLeftUp,

    frontRightDown,
    frontLeftDown,
    backRightDown,
    backLeftDown,
};

struct ChunkRegion {
  public:
    std::shared_ptr<Chunk> main;
    std::optional<std::shared_ptr<Chunk>> right;
    std::optional<std::shared_ptr<Chunk>> left;
    std::optional<std::shared_ptr<Chunk>> front;
    std::optional<std::shared_ptr<Chunk>> back;

    std::optional<std::shared_ptr<Chunk>> frontRight;
    std::optional<std::shared_ptr<Chunk>> backRight;
    std::optional<std::shared_ptr<Chunk>> frontLeft;
    std::optional<std::shared_ptr<Chunk>> backLeft;

    unsigned char getBlockFaces(int x, int y, int z);

    std::array<float, 8> getBlockOcclusion(int x, int y, int z);

  private:
    Block getBlock(int x, int y, int z);
};

#endif
