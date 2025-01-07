#include "chunk.h"

#include <cmath>

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

Chunk Chunk::generateChunk(int x, int z) {
    Chunk chunk = Chunk(x, z);
    for (int i = 0; i < CHUNK_WIDTH; ++i) {
        for (int j = 0; j < CHUNK_HEIGHT; ++j) {
            for (int k = 0; k < CHUNK_WIDTH; ++k) {
                chunk.blocks[i][j][k] = j < CHUNK_HEIGHT/3+2*(cos((i+k*sin(x+z))/4*cos(((z+x)*CHUNK_WIDTH+k+j)/10))+cos((k-i*sin(z+x))/3))? Block(1, false) : Block();
            }
        }
    }
    return chunk;
}

void Chunk::setBlock(int x, int y, int z, Block block){
    blocks[x][y][z] = block;
}

void Chunk::updateBlock(int x, int y, int z, const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk) {
    if(blocks[x][y][z].ID==0){
        blocks[x][y][z].faces = 0;
        return;
    }

    unsigned char newFaces = ALL_FACE;
    if(x != CHUNK_WIDTH-1? blocks[x+1][y][z].ID!=0 : rightChunk? rightChunk->blocks[0][y][z].ID!=0 : false)
        newFaces &= ~RIGHT_FACE;
    if(x != 0? blocks[x-1][y][z].ID!=0 : leftChunk? leftChunk->blocks[CHUNK_WIDTH-1][y][z].ID!=0 : false)
        newFaces &= ~LEFT_FACE;
    if(z != CHUNK_WIDTH-1? blocks[x][y][z+1].ID!=0 : frontChunk? frontChunk->blocks[x][y][0].ID!=0 : false)
        newFaces &= ~FRONT_FACE;
    if(z != 0? blocks[x][y][z-1].ID!=0 : backChunk? backChunk->blocks[x][y][CHUNK_WIDTH-1].ID!=0 : false)
        newFaces &= ~BACK_FACE;
    if(y != CHUNK_HEIGHT-1 && blocks[x][y+1][z].ID!=0)
        newFaces &= ~UP_FACE;
    if(y != 0 && blocks[x][y-1][z].ID!=0)
        newFaces &= ~DOWN_FACE;
        
    blocks[x][y][z].faces = newFaces;
}


void Chunk::updateBlocks(const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk) {

    for (int x = 0; x < CHUNK_WIDTH; x++)
        for (int y = 0; y < CHUNK_HEIGHT; y++)
            for (int z = 0; z < CHUNK_WIDTH; z++){
                if(blocks[x][y][z].ID==0){
                    blocks[x][y][z].faces = 0;
                    continue;
                }
                unsigned char newFaces = ALL_FACE;

                if(x != CHUNK_WIDTH-1? blocks[x+1][y][z].ID!=0 : rightChunk? rightChunk->blocks[0][y][z].ID!=0 : false)
                    newFaces &= ~RIGHT_FACE;
                if(x != 0? blocks[x-1][y][z].ID!=0 : leftChunk? leftChunk->blocks[CHUNK_WIDTH-1][y][z].ID!=0 : false)
                    newFaces &= ~LEFT_FACE;

                if(z != CHUNK_WIDTH-1? blocks[x][y][z+1].ID!=0 : frontChunk? frontChunk->blocks[x][y][0].ID!=0 : false)
                    newFaces &= ~FRONT_FACE;
                if(z != 0? blocks[x][y][z-1].ID!=0 : backChunk? backChunk->blocks[x][y][CHUNK_WIDTH-1].ID!=0 : false)
                    newFaces &= ~BACK_FACE;
                    
                if(y != CHUNK_HEIGHT-1 && blocks[x][y+1][z].ID!=0)
                    newFaces &= ~UP_FACE;
                if(y != 0 && blocks[x][y-1][z].ID!=0)
                    newFaces &= ~DOWN_FACE;

                blocks[x][y][z].faces = newFaces;
            }
}

/*

//Ugly piece of code to see,
//but beautifully efficient at the same time.
//
//Updates all block on chunk, including corners.
//For faster performance, each nested for-loop has lesser if statements and more manual tweaks

//Too much work for not enogh reward, but enough work to make me store it here

    for (x = 1; x < CHUNK_WIDTH-1; x++)
        for (y = 1; y < CHUNK_HEIGHT-1; y++)
            for (z = 1; z < CHUNK_WIDTH-1; z++){
                if(blocks[x][y][z].ID==0){
                    blocks[x][y][z].faces = 0;
                    continue;;
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
    
    for (x = 0; x < CHUNK_WIDTH; x += CHUNK_WIDTH-1)
        for (y = 1; y < CHUNK_HEIGHT-1; y++)
            for (z = 1; z < CHUNK_WIDTH-1; z++){
                if(blocks[x][y][z].ID==0){
                    blocks[x][y][z].faces = 0;
                    continue;
                }
                unsigned char newFaces = ALL_FACE;
                if(!x? blocks[x+1][y][z].ID!=0 : rightChunk? rightChunk->blocks[0][y][z].ID!=0 : false)
                    newFaces &= ~RIGHT_FACE;
                if(x? blocks[x-1][y][z].ID!=0 : leftChunk? leftChunk->blocks[CHUNK_WIDTH-1][y][z].ID!=0 : false)
                    newFaces &= ~LEFT_FACE;
                if(blocks[x][y+1][z].ID!=0)
                    newFaces &= ~UP_FACE;
                if(blocks[x][y-1][z].ID!=0)
                    newFaces &= ~DOWN_FACE;
                if(blocks[x][y][z+1].ID!=0)
                    newFaces &= ~FRONT_FACE;
                if(blocks[x][y][z-1].ID!=0)
                    newFaces &= ~BACK_FACE;
                blocks[x][y][z].faces = newFaces;
            }

    for (x = 1; x < CHUNK_WIDTH-1; x++)
        for (y = 1; y < CHUNK_HEIGHT-1; y++)
            for (z = 0; z < CHUNK_WIDTH; z += CHUNK_WIDTH-1){
                if(blocks[x][y][z].ID==0){
                    blocks[x][y][z].faces = 0;
                    continue;
                }
                unsigned char newFaces = ALL_FACE;
                if(blocks[x+1][y][z].ID!=0)
                    newFaces &= ~RIGHT_FACE;
                if(blocks[x-1][y][z].ID!=0)
                    newFaces &= ~LEFT_FACE;
                if(blocks[x][y+1][z].ID!=0)
                    newFaces &= ~UP_FACE;
                if(blocks[x][y-1][z].ID!=0)
                    newFaces &= ~DOWN_FACE;
                if(!z? blocks[x][y][z+1].ID!=0 : frontChunk? frontChunk->blocks[x][y][0].ID!=0 : false)
                    newFaces &= ~FRONT_FACE;
                if(z? blocks[x][y][z-1].ID!=0 : backChunk? backChunk->blocks[x][y][CHUNK_WIDTH-1].ID!=0 : false)
                    newFaces &= ~BACK_FACE;
                blocks[x][y][z].faces = newFaces;
            }

    for (x = 1; x < CHUNK_WIDTH-1; x++)
        for (y = 0; y < CHUNK_HEIGHT; y += CHUNK_HEIGHT-1)
            for (z = 1; z < CHUNK_WIDTH-1; z++){
                if(blocks[x][y][z].ID==0){
                    blocks[x][y][z].faces = 0;
                    continue;
                }
                unsigned char newFaces = ALL_FACE;
                if(blocks[x+1][y][z].ID!=0)
                    newFaces &= ~RIGHT_FACE;
                if(blocks[x-1][y][z].ID!=0)
                    newFaces &= ~LEFT_FACE;
                if(!y && blocks[x][y+1][z].ID!=0)
                    newFaces &= ~UP_FACE;
                if(y && blocks[x][y-1][z].ID!=0)
                    newFaces &= ~DOWN_FACE;
                if(blocks[x][y][z+1].ID!=0)
                    newFaces &= ~FRONT_FACE;
                if(blocks[x][y][z-1].ID!=0)
                    newFaces &= ~BACK_FACE;
                blocks[x][y][z].faces = newFaces;
            }
*/