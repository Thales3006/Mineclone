#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "chunk.h"
#include <vector>

class ChunkManager {
    std::vector<Chunk> chunks;

    ChunkManager();

    void loadChunk(Chunk chunk);
    void unloadChunk(int x, int y);

    Chunk generateChunk(int x, int y);

};

#endif