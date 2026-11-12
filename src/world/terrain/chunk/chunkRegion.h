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
    frontRight,
    frontLeft,
    backRight,
    backLeft,
};

struct LockedRegion {
    std::shared_ptr<Chunk> main;
    std::shared_ptr<Chunk> right, left, front, back;
    std::shared_ptr<Chunk> frontRight, backRight, frontLeft, backLeft;

    unsigned char getBlockFaces(int x, int y, int z) const;

    std::array<float, 4> getBlockOcclusion(int x, int y, int z,
                                           unsigned char face) const;

  private:
    Block getBlock(int x, int y, int z) const;
};

struct ChunkRegion {
    std::weak_ptr<Chunk> main;
    std::weak_ptr<Chunk> right, left, front, back;
    std::weak_ptr<Chunk> frontRight, backRight, frontLeft, backLeft;

    std::optional<LockedRegion> lock() const;
};

#endif
