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

        void updateFaces();
        void updateCorner(Chunk& chunk,int x, int y, int z);

        void setBlock(int chunkx, int chunkz, int x, int y, int z, Block block);
};

#endif