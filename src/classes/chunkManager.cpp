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
        for (int x = 0; x < CHUNK_WIDTH; x++)
            for (int y = 0; y < CHUNK_HEIGHT; y++)
                for (int z = 0; z < CHUNK_WIDTH; z++)
                    chunk.updateBlock(x,y,z);
}

void ChunkManager::setBlock(int cx, int cy, int x, int y, int z, Block block){
    if((x >= 0 && x < CHUNK_WIDTH) || (y >= 0 && y < CHUNK_HEIGHT) || (z >= 0 && z < CHUNK_WIDTH))
        return;

    for(auto& chunk : chunks)
        if(cx == chunk.position[0] && cy == chunk.position[1]){
            chunk.setBlock(x,y,z, block);
            chunk.updateBlock(x+1,y,z);
            chunk.updateBlock(x-1,y,z);
            chunk.updateBlock(x,y+1,z);
            chunk.updateBlock(x,y-1,z);
            chunk.updateBlock(x,y,z+1);
            chunk.updateBlock(x,y,z-1);
        }
}