#ifndef CHUNK_MANAGER_CLASS_H
#define CHUNK_MANAGER_CLASS_H

#include "world/terrain/chunk/chunk.h"
#include "world/terrain/chunk/chunkRegion.h"

#include <array>
#include <atomic>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <sigslot/signal.hpp>
#include <tuple>

using chunk_map = std::map<std::tuple<int, int>, std::shared_ptr<Chunk>>;

class ChunkManager {
  public:
    std::mutex chunks_mutex;
    chunk_map chunks;

    sigslot::signal<RegionSnapshot> onChunkAdded;
    sigslot::signal<int, int> onChunkRemoved;
    sigslot::signal<RegionSnapshot, glm::ivec3> onChunkUpdated;

    ChunkManager(int seed);
    ~ChunkManager();

    bool isEmpty(int x, int z);

    std::optional<RegionSnapshot> snapshotRegion(int x, int z);

    void setBlock(int chunkx, int chunkz, int x, int y, int z, Block block);
    void setBlock(int chunkx, int chunkz, glm::vec3 pos, Block block);
    Block getBlock(int chunkx, int chunkz, glm::vec3 pos);

    void startLoading();
    void stopLoading();
    void updateLoading(int centerX, int centerZ);

    std::pair<std::mutex *, chunk_map *> unsafe_getChunkMap();

  private:
    int seed;

    std::atomic<bool> active;
    std::atomic<bool> fillInFlight;

    void fillChunkRadius(int radius, int chunkx, int chunkz);

    void loadChunk(std::shared_ptr<Chunk> chunk);
    void unloadChunk(int x, int z);

    void updateRegion(int chunkx, int chunkz, int x, int y, int z);
    std::shared_ptr<Chunk> getChunk(int x, int z);
    std::optional<RegionSnapshot> buildSnapshot(int x, int z);
};

#endif
