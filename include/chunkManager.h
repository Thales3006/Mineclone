#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "chunk.h"
#include <map>
#include <tuple>

class ChunkManager {
    public:
        std::map<std::tuple<int,int>, Chunk> chunks;

        ChunkManager();

        void loadChunk(Chunk chunk);
        void unloadChunk(int x, int z);

        void updateChunks(Chunk& chunk);
        void updateChunks();

        void setBlock(int chunkx, int chunkz, int x, int y, int z, Block block);
        void updateBlock(Chunk* chunk, int x, int y, int z);
};

#endif