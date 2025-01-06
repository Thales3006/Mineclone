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
}

void ChunkManager::unloadChunk(int x, int y) {
    for (auto it = chunks.begin(); it != chunks.end(); ++it)
        if (x == it->position[0] && y == it->position[1]) {
            chunks.erase(it);
            return;
        }
}