#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "chunk.h"
#include "mesh.h"
#include <array>
#include <map>
#include <tuple>

class ChunkManager {
  public:
    std::map<std::tuple<int, int>, Chunk> chunks;

    ChunkManager();

    void loadChunk(Chunk chunk);
    void unloadChunk(int x, int z);

    void updateChunk(Chunk &chunk);
    void updateChunks();
    void updateSide(const Side side, Chunk &chunk);

    void setBlock(int chunkx, int chunkz, int x, int y, int z, Block block);
    void setBlock(int chunkx, int chunkz, glm::vec3 pos, Block block);
    void updateBlock(Chunk &chunk, int x, int y, int z);
    void updateRegion(Chunk &chunk, int x, int y, int z);

    Block getBlock(int chunkx, int chunkz, glm::vec3 pos);

    void renderChunks(Shader &shader, int chunkx, int chunkz);

    void fillChunkRadius(int radius, int chunkx, int chunkz);
};

#endif
