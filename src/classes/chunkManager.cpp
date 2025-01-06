#include "chunkManager.h"

#include <cmath>
#include <iostream>
ChunkManager::ChunkManager() {}

void ChunkManager::loadChunk(Chunk chunk) {
    if(chunks.find({chunk.x, chunk.z}) != chunks.end())
        return;
    chunks[{chunk.x, chunk.z}] = chunk;
    updateFaces();
}

void ChunkManager::unloadChunk(int x, int z) {
    if(chunks.find({x, z}) == chunks.end())
        return;
    chunks.erase({x, z});
}

void ChunkManager::updateFaces(){
    for (auto& [coords, chunk] : chunks)
        for (int x = 0; x < CHUNK_WIDTH; x++)
            for (int y = 0; y < CHUNK_HEIGHT; y++)
                for (int z = 0; z < CHUNK_WIDTH; z++)
                    chunk.updateBlock(x,y,z);
}

void ChunkManager::setBlock(int chunkx, int chunkz, int x, int y, int z, Block block){
    if((chunks.find({chunkx, chunkz}) == chunks.end()) || (x < 0 || x >= CHUNK_WIDTH) || (y < 0 || y >= CHUNK_HEIGHT) || (z < 0 || z >= CHUNK_WIDTH))
        return;

    Chunk& chunk = chunks[{chunkx, chunkz}];
    chunk.setBlock(x,y,z, block);
    
    chunk.updateBlock(x+1,y,z);
    chunk.updateBlock(x-1,y,z);
    chunk.updateBlock(x,y+1,z);
    chunk.updateBlock(x,y-1,z);
    chunk.updateBlock(x,y,z+1);
    chunk.updateBlock(x,y,z-1);
}
/*
void ChunkManager::updateCorner(Chunk& chunk,int x, int y, int z){
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
}*/