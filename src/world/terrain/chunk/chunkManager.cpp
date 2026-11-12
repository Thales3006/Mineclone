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

void ChunkManager::updateLoading(int centerX, int centerZ) {
    if (!active || fillInFlight)
        return;

    fillInFlight = true;
    TaskManager::instance().run([this, centerX, centerZ]() {
        fillChunkRadius(6, centerX, centerZ);
        fillInFlight = false;
    });
}

std::shared_ptr<Chunk> ChunkManager::getChunk(int x, int z) {
    auto it = chunks.find({x, z});
    if (it != chunks.end()) {
        return it->second;
    }
    return nullptr;
}

void ChunkManager::loadChunk(std::shared_ptr<Chunk> chunk) {
    if (chunks.find({chunk->x, chunk->z}) != chunks.end())
        return;

    chunks[{chunk->x, chunk->z}] = std::move(chunk);
}

void ChunkManager::unloadChunk(int x, int z) {
    if (chunks.find({x, z}) == chunks.end())
        return;

    onChunkRemoved(x, z);
    chunks.erase({x, z});
}

void ChunkManager::updateRegion(int chunkx, int chunkz, int x, int y, int z) {
    glm::ivec3 worldEdit = {chunkx * CHUNK_WIDTH + x, y,
                            chunkz * CHUNK_WIDTH + z};

    auto emit = [this, worldEdit](int cx, int cz) {
        if (auto snapshot = buildSnapshot(cx, cz))
            onChunkUpdated(std::move(*snapshot), worldEdit);
    };

    emit(chunkx, chunkz);

    bool atLeft = (x == 0);
    bool atRight = (x == CHUNK_WIDTH - 1);
    bool atBack = (z == 0);
    bool atFront = (z == CHUNK_WIDTH - 1);

    if (atRight)
        emit(chunkx + 1, chunkz);
    if (atLeft)
        emit(chunkx - 1, chunkz);
    if (atFront)
        emit(chunkx, chunkz + 1);
    if (atBack)
        emit(chunkx, chunkz - 1);

    if (atFront && atRight)
        emit(chunkx + 1, chunkz + 1);
    if (atBack && atRight)
        emit(chunkx + 1, chunkz - 1);
    if (atFront && atLeft)
        emit(chunkx - 1, chunkz + 1);
    if (atBack && atLeft)
        emit(chunkx - 1, chunkz - 1);
}

void ChunkManager::setBlock(int chunkx, int chunkz, int x, int y, int z,
                            Block block) {
    std::lock_guard<std::mutex> lock(chunks_mutex);
    if ((chunks.find({chunkx, chunkz}) == chunks.end()) ||
        (x < 0 || x >= CHUNK_WIDTH) || (y < 0 || y >= CHUNK_HEIGHT) ||
        (z < 0 || z >= CHUNK_WIDTH))
        return;

    chunks.at({chunkx, chunkz})->setBlock(x, y, z, block);

    updateRegion(chunkx, chunkz, x, y, z);
}

void ChunkManager::setBlock(int chunkx, int chunkz, glm::vec3 pos,
                            Block block) {
    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunkx += chunkOffset.x;
    chunkz += chunkOffset.z;
    setBlock(chunkx, chunkz, (int)pos.x, (int)pos.y, (int)pos.z, block);
}

Block ChunkManager::getBlock(int chunkx, int chunkz, glm::vec3 pos) {
    std::lock_guard<std::mutex> lock(chunks_mutex);

    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunkx += chunkOffset.x;
    chunkz += chunkOffset.z;

    if ((chunks.find({chunkx, chunkz}) == chunks.end()))
        return Block();

    return chunks[{chunkx, chunkz}]->blocks[int(pos.x)][int(pos.y)][int(pos.z)];
}

void ChunkManager::fillChunkRadius(int radius, int chunkx, int chunkz) {
    std::vector<std::tuple<int, int>> to_delete{};
    std::vector<std::tuple<int, int>> to_add{};

    {
        std::lock_guard<std::mutex> lock(chunks_mutex);

        for (auto &[coord, chunk] : chunks) {
            int x = std::get<0>(coord);
            int z = std::get<1>(coord);

            if (x < -radius + chunkx || x > radius + chunkx ||
                z < -radius + chunkz || z > radius + chunkz) {
                to_delete.push_back(coord);
            }
        }

        for (int i = -radius + chunkx; i <= radius + chunkx; i++) {
            for (int j = -radius + chunkz; j <= radius + chunkz; j++) {
                if (chunks.find({i, j}) == chunks.end()) {
                    to_add.push_back({i, j});
                }
            }
        }
    }

    std::vector<std::shared_ptr<Chunk>> to_add_chunks{};
    for (auto &coord : to_add) {
        int x = std::get<0>(coord);
        int z = std::get<1>(coord);
        to_add_chunks.push_back(Chunk::generateChunk(seed, x, z));
    }

    {
        std::lock_guard<std::mutex> lock(chunks_mutex);

        for (auto &coord : to_delete) {
            int x = std::get<0>(coord);
            int z = std::get<1>(coord);
            unloadChunk(x, z);
        }

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
            if (auto snapshot =
                    buildSnapshot(std::get<0>(coord), std::get<1>(coord)))
                onChunkAdded(std::move(*snapshot));
    }
}

bool ChunkManager::isEmpty(int x, int z) {
    std::lock_guard<std::mutex> lock(chunks_mutex);

    return !getChunk(x, z);
}

std::optional<RegionSnapshot> ChunkManager::buildSnapshot(int x, int z) {
    std::shared_ptr<Chunk> main = getChunk(x, z);
    if (!main)
        return std::nullopt;

    return RegionSnapshot{
        .main = std::move(main),
        .right = getChunk(x + 1, z),
        .left = getChunk(x - 1, z),
        .front = getChunk(x, z + 1),
        .back = getChunk(x, z - 1),
        .frontRight = getChunk(x + 1, z + 1),
        .backRight = getChunk(x + 1, z - 1),
        .frontLeft = getChunk(x - 1, z + 1),
        .backLeft = getChunk(x - 1, z - 1),
    };
}

std::optional<RegionSnapshot> ChunkManager::snapshotRegion(int x, int z) {
    std::lock_guard<std::mutex> lock(chunks_mutex);
    return buildSnapshot(x, z);
}

std::pair<std::mutex *, chunk_map *> ChunkManager::unsafe_getChunkMap() {
    return std::make_pair(&chunks_mutex, &chunks);
}
