#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "world/chunk.h"

#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <tuple>

class ChunkManager {
  public:
    std::mutex chunks_mutex;
    std::map<std::tuple<int, int>, std::unique_ptr<Chunk>> chunks;

    ChunkManager();

    void setBlock(int chunkx, int chunkz, int x, int y, int z, Block block);
    void setBlock(int chunkx, int chunkz, glm::vec3 pos, Block block);
    Block getBlock(int chunkx, int chunkz, glm::vec3 pos);

    void renderChunks(Shader &shader, int chunkx, int chunkz);

    void fillChunkRadius(int radius, int chunkx, int chunkz);

    void updateChunks();

  private:
    void loadChunk(std::unique_ptr<Chunk> chunk);
    void unloadChunk(int x, int z);

    void updateChunk(Chunk &chunk);
    void updateSide(const Side side, Chunk &chunk);

    void updateBlock(Chunk &chunk, int x, int y, int z);
    void updateRegion(Chunk &chunk, int x, int y, int z);

    Chunk *getChunkPtr(int x, int z);
};

#endif
