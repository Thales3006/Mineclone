#include "chunk.h"

#include <cmath>

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

Chunk Chunk::generateChunk(int x, int y) {
    Chunk chunk = Chunk(x, y);
    for (int i = 0; i < CHUNK_WIDTH; ++i) {
        for (int j = 0; j < CHUNK_HEIGHT; ++j) {
            for (int k = 0; k < CHUNK_WIDTH; ++k) {
                chunk.blocks[i][j][k] = j < CHUNK_HEIGHT/3+2*(cos((i+k*sin(x+y))/4*cos(((y+x)*CHUNK_WIDTH+k+j)/10))+cos((k-i*sin(y+x))/3))? Block(1, true) : Block();
            }
        }
    }
    return chunk;
}