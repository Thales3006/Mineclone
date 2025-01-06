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

Chunk ChunkManager::generateChunk(int x, int y) {
    Chunk chunk = Chunk(x, y);
    for (int i = 0; i < CHUNK_WIDTH; ++i) {
        for (int j = 0; j < CHUNK_HEIGHT; ++j) {
            for (int k = 0; k < CHUNK_WIDTH; ++k) {
                chunk.blocks[i][j][k] = j<CHUNK_HEIGHT/2? Block(glm::vec3(i,j,k), 1, true) : Block(glm::vec3(i,j,k), 0, false);
            }
        }
    }
    return chunk;
}