#include "chunkManager.h"

#include <cmath>

ChunkManager::ChunkManager() {
    chunks = std::vector<Chunk>();
}

void ChunkManager::loadChunk(Chunk chunk) {
    for(const auto& oldChunk : chunks)
        if(chunk.position[0] == oldChunk.position[0] && chunk.position[1] == oldChunk.position[1])
            return;
    chunks.push_back(chunk);
    updateFaces();
}

void ChunkManager::unloadChunk(int x, int y) {
    for (auto it = chunks.begin(); it != chunks.end(); ++it)
        if (x == it->position[0] && y == it->position[1]) {
            chunks.erase(it);
            return;
        }
}

void ChunkManager::updateFaces(){
    for(auto& chunk : chunks)
        for (int x = 1; x < CHUNK_WIDTH-1; x++)
            for (int y = 1; y < CHUNK_HEIGHT-1; y++)
                for (int z = 1; z < CHUNK_WIDTH-1; z++) {
                    unsigned char newFaces = ALL_FACE;
                    if(chunk.blocks[x+1][y][z].ID!=0)newFaces &= ~RIGHT_FACE;
                    if(chunk.blocks[x-1][y][z].ID!=0)newFaces &= ~LEFT_FACE;
                    if(chunk.blocks[x][y+1][z].ID!=0)newFaces &= ~UP_FACE;
                    if(chunk.blocks[x][y-1][z].ID!=0)newFaces &= ~DOWN_FACE;
                    if(chunk.blocks[x][y][z+1].ID!=0)newFaces &= ~FRONT_FACE;
                    if(chunk.blocks[x][y][z-1].ID!=0)newFaces &= ~BACK_FACE;
                    chunk.blocks[x][y][z].faces = newFaces;
                }
}

void ChunkManager::setBlock(int cx, int cy, int x, int y, int z, Block block){
    for(auto& chunk : chunks)
        if(cx == chunk.position[0] && cy == chunk.position[1]){
            unsigned char newFaces = ALL_FACE;
            if(chunk.blocks[x+1][y][z].ID!=0)newFaces &= ~RIGHT_FACE;
            if(chunk.blocks[x-1][y][z].ID!=0)newFaces &= ~LEFT_FACE;
            if(chunk.blocks[x][y+1][z].ID!=0)newFaces &= ~UP_FACE;
            if(chunk.blocks[x][y-1][z].ID!=0)newFaces &= ~DOWN_FACE;
            if(chunk.blocks[x][y][z+1].ID!=0)newFaces &= ~FRONT_FACE;
            if(chunk.blocks[x][y][z-1].ID!=0)newFaces &= ~BACK_FACE;

            block.faces = newFaces;
            chunk.blocks[x][y][z] = block;
            return;
        }
}