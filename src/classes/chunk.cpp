#include "chunk.h"

Chunk::Chunk() {
    position[0] = 0;
    position[1] = 0;
}

Chunk::Chunk(int x, int y) {
    position[0] = x;
    position[1] = y;
}

Chunk::Chunk(int x, int y, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]){
    position[0] = x;
    position[1] = y;

    for (int h = 0; h < CHUNK_WIDTH; ++h) {
        for (int w = 0; w < CHUNK_HEIGHT; ++w) {
            for (int d = 0; d < CHUNK_WIDTH; ++d) {
                this->blocks[h][w][d] = blocks[h][w][d];
            }
        }
    }
}