#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "chunk.h"
#include <vector>

class ChunkManager {
    public:
        std::vector<Chunk> chunks;

        ChunkManager();

        void loadChunk(Chunk chunk);
        void unloadChunk(int x, int y);

        void updateFaces();

        void setBlock(int cx, int cy, int x, int y, int z, Block block);
};

#endif