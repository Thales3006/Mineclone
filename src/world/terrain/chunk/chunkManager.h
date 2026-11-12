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
    sigslot::signal<glm::ivec2> onChunkRemoved;
    sigslot::signal<RegionSnapshot, glm::ivec3> onChunkUpdated;

    ChunkManager(int seed);
    ~ChunkManager();

    bool isEmpty(glm::ivec2 coord);

    std::optional<RegionSnapshot> snapshotRegion(glm::ivec2 coord);

    void setBlock(glm::ivec2 chunk, glm::ivec3 block, Block value);
    void setBlock(glm::ivec2 chunk, glm::vec3 pos, Block value);
    Block getBlock(glm::ivec2 chunk, glm::vec3 pos);

    void startLoading();
    void stopLoading();
    void updateLoading(glm::ivec2 center);

    std::pair<std::mutex *, chunk_map *> unsafe_getChunkMap();

  private:
    int seed;

    std::atomic<bool> active;
    std::atomic<bool> fillInFlight;

    void fillChunkRadius(int radius, glm::ivec2 center);

    void loadChunk(std::shared_ptr<Chunk> chunk);
    void unloadChunk(glm::ivec2 coord);

    void updateRegion(glm::ivec2 chunk, glm::ivec3 block);
    std::shared_ptr<Chunk> getChunk(glm::ivec2 coord);
    std::optional<RegionSnapshot> buildSnapshot(glm::ivec2 coord);
};

#endif
