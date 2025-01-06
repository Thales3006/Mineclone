#include "chunk.h"

Chunk::Chunk() {
    position[0] = 0;
    position[1] = 0;
}

Chunk::Chunk(int x, int y) {
    position[0] = x;
    position[1] = y;
}