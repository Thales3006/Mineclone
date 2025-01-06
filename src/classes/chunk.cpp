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
                chunk.blocks[i][j][k] = j < CHUNK_HEIGHT/3+2*(cos((i+k*sin(x+y))/4*cos(((y+x)*CHUNK_WIDTH+k+j)/10))+cos((k-i*sin(y+x))/3))? Block(1, false) : Block();
            }
        }
    }
    return chunk;
}

void Chunk::setBlock(int x, int y, int z, Block block){
    blocks[x][y][z] = block;
    updateBlock(x,y,z);
}

void Chunk::updateBlock(int x, int y, int z){
    if(blocks[x][y][z].ID==0){
        blocks[x][y][z].faces = 0;
        return;
    }

    unsigned char newFaces = ALL_FACE;
    if(x != CHUNK_WIDTH-1 && blocks[x+1][y][z].ID!=0)
        newFaces &= ~RIGHT_FACE;
    if(x != 0 && blocks[x-1][y][z].ID!=0)
        newFaces &= ~LEFT_FACE;
    if(y != CHUNK_HEIGHT-1 && blocks[x][y+1][z].ID!=0)
        newFaces &= ~UP_FACE;
    if(y != 0 && blocks[x][y-1][z].ID!=0)
        newFaces &= ~DOWN_FACE;
    if(z != CHUNK_WIDTH-1 && blocks[x][y][z+1].ID!=0)
        newFaces &= ~FRONT_FACE;
    if(z != 0 && blocks[x][y][z-1].ID!=0)
        newFaces &= ~BACK_FACE;

    blocks[x][y][z].faces = newFaces;
}