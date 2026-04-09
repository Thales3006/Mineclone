#include "world/terrain/chunk/chunkManager.h"

ChunkManager::ChunkManager() {}

std::optional<std::shared_ptr<Chunk>> ChunkManager::getChunk(int x, int z) {
    auto it = chunks.find({x, z});
    if (it != chunks.end()) {
        return std::make_optional(it->second);
    }
    return std::nullopt;
}

ChunkRegion ChunkManager::getChunkRegion(int x, int z) {
    return ChunkRegion{
        .main = std::move(getChunk(x, z).value()),
        .right = std::move(getChunk(x + 1, z)),
        .left = std::move(getChunk(x - 1, z)),
        .front = std::move(getChunk(x, z + 1)),
        .back = std::move(getChunk(x, z - 1)),
    };
}

void ChunkManager::loadChunk(std::shared_ptr<Chunk> chunk) {
    if (chunks.find({chunk->x, chunk->z}) != chunks.end())
        return;

    int x = chunk->x;
    int z = chunk->z;
    chunks[{x, z}] = std::move(chunk);

    onChunkAdded(std::move(getChunkRegion(x, z)));
}

void ChunkManager::unloadChunk(int x, int z) {
    if (chunks.find({x, z}) == chunks.end())
        return;

    onChunkRemoved(std::move(getChunkRegion(x, z)));
    chunks.erase({x, z});
}

void ChunkManager::updateRegion(ChunkRegion region) {

    onChunkUpdated(region);

    if (region.right.has_value()) {
        auto chunk = region.right.value();
        onChunkUpdated(std::move(getChunkRegion(chunk->x, chunk->z)));
    }
    if (region.left.has_value()) {
        auto chunk = region.left.value();
        onChunkUpdated(std::move(getChunkRegion(chunk->x, chunk->z)));
    }
    if (region.front.has_value()) {
        auto chunk = region.front.value();
        onChunkUpdated(std::move(getChunkRegion(chunk->x, chunk->z)));
    }
    if (region.back.has_value()) {
        auto chunk = region.back.value();
        onChunkUpdated(std::move(getChunkRegion(chunk->x, chunk->z)));
    }
}

void ChunkManager::setBlock(int chunkx, int chunkz, int x, int y, int z,
                            Block block) {
    std::lock_guard<std::mutex> lock(chunks_mutex);
    if ((chunks.find({chunkx, chunkz}) == chunks.end()) ||
        (x < 0 || x >= CHUNK_WIDTH) || (y < 0 || y >= CHUNK_HEIGHT) ||
        (z < 0 || z >= CHUNK_WIDTH))
        return;

    ChunkRegion region = getChunkRegion(chunkx, chunkz);

    region.main->setBlock(x, y, z, block);

    updateRegion(std::move(region));
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
        to_add_chunks.push_back(std::move(Chunk::generateChunk(x, z)));
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
    }
}

bool ChunkManager::isEmpty(int x, int z) {
    std::lock_guard<std::mutex> lock(chunks_mutex);

    return !getChunk(x, z).has_value();
}

std::pair<std::mutex *, chunk_map *> ChunkManager::unsafe_getChunkMap() {
    return std::make_pair(&chunks_mutex, &chunks);
}
