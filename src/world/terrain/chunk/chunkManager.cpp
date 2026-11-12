#include "world/terrain/chunk/chunkManager.h"

#include "game/task/taskManager.h"

#include <chrono>
#include <set>
#include <thread>

ChunkManager::ChunkManager(int seed)
    : seed(seed), active(false), fillInFlight(false) {}

ChunkManager::~ChunkManager() { stopLoading(); }

void ChunkManager::startLoading() { active = true; }

void ChunkManager::stopLoading() {
    active = false;
    while (fillInFlight)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void ChunkManager::updateLoading(glm::ivec2 center) {
    if (!active || fillInFlight)
        return;

    fillInFlight = true;
    TaskManager::instance().run([this, center]() {
        fillChunkRadius(12, center);
        fillInFlight = false;
    });
}

std::shared_ptr<Chunk> ChunkManager::getChunk(glm::ivec2 coord) {
    auto it = chunks.find({coord.x, coord.y});
    if (it != chunks.end()) {
        return it->second;
    }
    return nullptr;
}

void ChunkManager::loadChunk(std::shared_ptr<Chunk> chunk) {
    if (chunks.find({chunk->coord.x, chunk->coord.y}) != chunks.end())
        return;

    chunks[{chunk->coord.x, chunk->coord.y}] = std::move(chunk);
}

void ChunkManager::unloadChunk(glm::ivec2 coord) {
    if (chunks.find({coord.x, coord.y}) == chunks.end())
        return;

    onChunkRemoved(coord);
    chunks.erase({coord.x, coord.y});
}

void ChunkManager::updateRegion(glm::ivec2 chunk, glm::ivec3 block) {
    glm::ivec3 worldEdit = {chunk.x * CHUNK_WIDTH + block.x, block.y,
                            chunk.y * CHUNK_WIDTH + block.z};

    auto emit = [this, worldEdit](glm::ivec2 c) {
        if (auto snapshot = buildSnapshot(c))
            onChunkUpdated(std::move(*snapshot), worldEdit);
    };

    emit(chunk);

    bool atLeft = (block.x == 0);
    bool atRight = (block.x == CHUNK_WIDTH - 1);
    bool atBack = (block.z == 0);
    bool atFront = (block.z == CHUNK_WIDTH - 1);

    if (atRight)
        emit(chunk + glm::ivec2(1, 0));
    if (atLeft)
        emit(chunk + glm::ivec2(-1, 0));
    if (atFront)
        emit(chunk + glm::ivec2(0, 1));
    if (atBack)
        emit(chunk + glm::ivec2(0, -1));

    if (atFront && atRight)
        emit(chunk + glm::ivec2(1, 1));
    if (atBack && atRight)
        emit(chunk + glm::ivec2(1, -1));
    if (atFront && atLeft)
        emit(chunk + glm::ivec2(-1, 1));
    if (atBack && atLeft)
        emit(chunk + glm::ivec2(-1, -1));
}

void ChunkManager::setBlock(glm::ivec2 chunk, glm::ivec3 block, Block value) {
    std::lock_guard<std::mutex> lock(chunks_mutex);
    if ((chunks.find({chunk.x, chunk.y}) == chunks.end()) ||
        (block.x < 0 || block.x >= CHUNK_WIDTH) ||
        (block.y < 0 || block.y >= CHUNK_HEIGHT) ||
        (block.z < 0 || block.z >= CHUNK_WIDTH))
        return;

    chunks.at({chunk.x, chunk.y})->setBlock(block, value);

    updateRegion(chunk, block);
}

void ChunkManager::setBlock(glm::ivec2 chunk, glm::vec3 pos, Block value) {
    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunk += glm::ivec2(chunkOffset.x, chunkOffset.z);
    setBlock(chunk, glm::ivec3((int)pos.x, (int)pos.y, (int)pos.z), value);
}

Block ChunkManager::getBlock(glm::ivec2 chunk, glm::vec3 pos) {
    std::lock_guard<std::mutex> lock(chunks_mutex);

    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunk += glm::ivec2(chunkOffset.x, chunkOffset.z);

    if ((chunks.find({chunk.x, chunk.y}) == chunks.end()))
        return Block();

    return chunks[{chunk.x, chunk.y}]->blocks[int(pos.x)][int(pos.y)]
                                             [int(pos.z)];
}

void ChunkManager::fillChunkRadius(int radius, glm::ivec2 center) {
    std::vector<std::tuple<int, int>> to_delete{};
    std::vector<std::tuple<int, int>> to_add{};

    {
        std::lock_guard<std::mutex> lock(chunks_mutex);

        for (auto &[coord, chunk] : chunks) {
            int x = std::get<0>(coord);
            int z = std::get<1>(coord);

            if (x < -radius + center.x || x > radius + center.x ||
                z < -radius + center.y || z > radius + center.y) {
                to_delete.push_back(coord);
            }
        }

        for (int i = -radius + center.x; i <= radius + center.x; i++) {
            for (int j = -radius + center.y; j <= radius + center.y; j++) {
                if (chunks.find({i, j}) == chunks.end()) {
                    to_add.push_back({i, j});
                }
            }
        }
    }

    std::vector<std::shared_ptr<Chunk>> to_add_chunks{};
    for (auto &coord : to_add)
        to_add_chunks.push_back(Chunk::generateChunk(
            seed, glm::ivec2(std::get<0>(coord), std::get<1>(coord))));

    {
        std::lock_guard<std::mutex> lock(chunks_mutex);

        for (auto &coord : to_delete)
            unloadChunk(glm::ivec2(std::get<0>(coord), std::get<1>(coord)));

        for (auto &chunk : to_add_chunks) {
            loadChunk(std::move(chunk));
        }

        std::set<std::tuple<int, int>> affected;
        auto markNeighborhood = [&affected](int x, int z) {
            for (int dx = -1; dx <= 1; dx++)
                for (int dz = -1; dz <= 1; dz++)
                    affected.insert({x + dx, z + dz});
        };
        for (auto &coord : to_add)
            markNeighborhood(std::get<0>(coord), std::get<1>(coord));
        for (auto &coord : to_delete)
            markNeighborhood(std::get<0>(coord), std::get<1>(coord));

        for (auto &coord : affected)
            if (auto snapshot = buildSnapshot(
                    glm::ivec2(std::get<0>(coord), std::get<1>(coord))))
                onChunkAdded(std::move(*snapshot));
    }
}

bool ChunkManager::isEmpty(glm::ivec2 coord) {
    std::lock_guard<std::mutex> lock(chunks_mutex);

    return !getChunk(coord);
}

std::optional<RegionSnapshot> ChunkManager::buildSnapshot(glm::ivec2 coord) {
    std::shared_ptr<Chunk> main = getChunk(coord);
    if (!main)
        return std::nullopt;

    return RegionSnapshot{
        .main = std::move(main),
        .right = getChunk(coord + glm::ivec2(1, 0)),
        .left = getChunk(coord + glm::ivec2(-1, 0)),
        .front = getChunk(coord + glm::ivec2(0, 1)),
        .back = getChunk(coord + glm::ivec2(0, -1)),
        .frontRight = getChunk(coord + glm::ivec2(1, 1)),
        .backRight = getChunk(coord + glm::ivec2(1, -1)),
        .frontLeft = getChunk(coord + glm::ivec2(-1, 1)),
        .backLeft = getChunk(coord + glm::ivec2(-1, -1)),
    };
}

std::optional<RegionSnapshot> ChunkManager::snapshotRegion(glm::ivec2 coord) {
    std::lock_guard<std::mutex> lock(chunks_mutex);
    return buildSnapshot(coord);
}

std::pair<std::mutex *, chunk_map *> ChunkManager::unsafe_getChunkMap() {
    return std::make_pair(&chunks_mutex, &chunks);
}
