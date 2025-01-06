#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 256

class Chunk {
    public:
        int position[2];
        Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

        Chunk();
        Chunk(int x, int y);
        Chunk(int x, int y, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]);
}; 

#endif