#include "world/chunkManager.h"

ChunkManager::ChunkManager() {}

Chunk *ChunkManager::getChunkPtr(int x, int z) {
    auto it = chunks.find({x, z});
    if (it != chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

void ChunkManager::loadChunk(std::unique_ptr<Chunk> chunk) {
    if (chunks.find({chunk->x, chunk->z}) != chunks.end())
        return;

    int x = chunk->x;
    int z = chunk->z;
    updateChunk(*chunk);
    chunks[{x, z}] = std::move(chunk);

    if (chunks.find({x - 1, z}) != chunks.end())
        updateSide(right, *chunks[{x - 1, z}]);
    if (chunks.find({x + 1, z}) != chunks.end())
        updateSide(left, *chunks[{x + 1, z}]);
    if (chunks.find({x, z + 1}) != chunks.end())
        updateSide(back, *chunks[{x, z + 1}]);
    if (chunks.find({x, z - 1}) != chunks.end())
        updateSide(front, *chunks[{x, z - 1}]);
}

void ChunkManager::unloadChunk(int x, int z) {
    if (chunks.find({x, z}) == chunks.end())
        return;

    chunks.erase({x, z});

    if (chunks.find({x - 1, z}) != chunks.end())
        updateSide(right, *chunks[{x - 1, z}]);
    if (chunks.find({x + 1, z}) != chunks.end())
        updateSide(left, *chunks[{x + 1, z}]);
    if (chunks.find({x, z + 1}) != chunks.end())
        updateSide(back, *chunks[{x, z + 1}]);
    if (chunks.find({x, z - 1}) != chunks.end())
        updateSide(front, *chunks[{x, z - 1}]);
}

void ChunkManager::updateSide(const Side side, Chunk &chunk) {
    Chunk *leftChunk = getChunkPtr(chunk.x - 1, chunk.z);
    Chunk *rightChunk = getChunkPtr(chunk.x + 1, chunk.z);
    Chunk *frontChunk = getChunkPtr(chunk.x, chunk.z + 1);
    Chunk *backChunk = getChunkPtr(chunk.x, chunk.z - 1);

    chunk.updateSide(side, leftChunk, rightChunk, frontChunk, backChunk);
}

void ChunkManager::updateChunk(Chunk &chunk) {
    Chunk *leftChunk = getChunkPtr(chunk.x - 1, chunk.z);
    Chunk *rightChunk = getChunkPtr(chunk.x + 1, chunk.z);
    Chunk *frontChunk = getChunkPtr(chunk.x, chunk.z + 1);
    Chunk *backChunk = getChunkPtr(chunk.x, chunk.z - 1);

    chunk.updateBlocks(leftChunk, rightChunk, frontChunk, backChunk);
    chunk.setMesh();
}

void ChunkManager::updateChunks() {
    for (auto &[coords, chunk] : chunks)
        updateChunk(*chunk);
}

void ChunkManager::updateBlock(Chunk &chunk, int x, int y, int z) {
    Chunk *leftChunk = getChunkPtr(chunk.x - 1, chunk.z);
    Chunk *rightChunk = getChunkPtr(chunk.x + 1, chunk.z);
    Chunk *frontChunk = getChunkPtr(chunk.x, chunk.z + 1);
    Chunk *backChunk = getChunkPtr(chunk.x, chunk.z - 1);

    chunk.updateBlock(x, y, z, leftChunk, rightChunk, frontChunk, backChunk);
}

void ChunkManager::updateRegion(Chunk &chunk, int x, int y, int z) {
    Chunk *leftChunk = getChunkPtr(chunk.x - 1, chunk.z);
    Chunk *rightChunk = getChunkPtr(chunk.x + 1, chunk.z);
    Chunk *frontChunk = getChunkPtr(chunk.x, chunk.z + 1);
    Chunk *backChunk = getChunkPtr(chunk.x, chunk.z - 1);

    chunk.updateBlock(x, y, z, leftChunk, rightChunk, frontChunk, backChunk);

    if (x + 1 < CHUNK_WIDTH)
        chunk.updateBlock(x + 1, y, z, leftChunk, rightChunk, frontChunk, backChunk);
    else if (rightChunk) {
        updateBlock(*rightChunk, 0, y, z);
        rightChunk->setMesh();
    }
    if (x - 1 >= 0)
        chunk.updateBlock(x - 1, y, z, leftChunk, rightChunk, frontChunk, backChunk);
    else if (leftChunk) {
        updateBlock(*leftChunk, CHUNK_WIDTH - 1, y, z);
        leftChunk->setMesh();
    }

    if (y + 1 < CHUNK_WIDTH)
        chunk.updateBlock(x, y + 1, z, leftChunk, rightChunk, frontChunk, backChunk);
    if (y - 1 >= 0)
        chunk.updateBlock(x, y - 1, z, leftChunk, rightChunk, frontChunk, backChunk);

    if (z + 1 < CHUNK_WIDTH)
        chunk.updateBlock(x, y, z + 1, leftChunk, rightChunk, frontChunk, backChunk);
    else if (frontChunk) {
        updateBlock(*frontChunk, x, y, 0);
        frontChunk->setMesh();
    }
    if (z - 1 >= 0)
        chunk.updateBlock(x, y, z - 1, leftChunk, rightChunk, frontChunk, backChunk);
    else if (backChunk) {
        updateBlock(*backChunk, x, y, CHUNK_WIDTH - 1);
        backChunk->setMesh();
    }
    chunk.setMesh();
}

void ChunkManager::setBlock(int chunkx, int chunkz, int x, int y, int z, Block block) {
    if ((chunks.find({chunkx, chunkz}) == chunks.end()) || (x < 0 || x >= CHUNK_WIDTH) ||
        (y < 0 || y >= CHUNK_HEIGHT) || (z < 0 || z >= CHUNK_WIDTH))
        return;

    Chunk &chunk = *chunks[{chunkx, chunkz}];

    chunk.setBlock(x, y, z, block);
    updateRegion(chunk, x, y, z);
}

void ChunkManager::setBlock(int chunkx, int chunkz, glm::vec3 pos, Block block) {
    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunkx += chunkOffset.x;
    chunkz += chunkOffset.z;

    if ((chunks.find({chunkx, chunkz}) == chunks.end()))
        return;

    Chunk &chunk = *chunks[{chunkx, chunkz}];

    chunk.setBlock(pos.x, pos.y, pos.z, block);
    updateRegion(chunk, pos.x, pos.y, pos.z);
}

Block ChunkManager::getBlock(int chunkx, int chunkz, glm::vec3 pos) {
    glm::vec3 chunkOffset = Chunk::chunkOffSet(pos);
    pos -= chunkOffset * float(CHUNK_WIDTH);
    chunkx += chunkOffset.x;
    chunkz += chunkOffset.z;

    if ((chunks.find({chunkx, chunkz}) == chunks.end()))
        return Block();

    return chunks[{chunkx, chunkz}]->blocks[int(pos.x)][int(pos.y)][int(pos.z)];
}

void ChunkManager::renderChunks(Shader &shader, int chunkx, int chunkz) {
    for (auto &[coord, chunk] : chunks)
        chunk->renderChunk(shader, chunkx, chunkz);
}

void ChunkManager::fillChunkRadius(int radius, int chunkx, int chunkz) {
    auto it = chunks.begin();
    while (it != chunks.end()) {
        const auto &[coord, chunk] = *it;
        int x = std::get<0>(coord);
        int z = std::get<1>(coord);
        if (x < -radius + chunkx || x > radius + chunkx || z < -radius + chunkz ||
            z > radius + chunkz) {
            it = chunks.erase(it);
            if (chunks.find({x - 1, z}) != chunks.end())
                updateSide(right, *chunks[{x - 1, z}]);
            if (chunks.find({x + 1, z}) != chunks.end())
                updateSide(left, *chunks[{x + 1, z}]);
            if (chunks.find({x, z + 1}) != chunks.end())
                updateSide(back, *chunks[{x, z + 1}]);
            if (chunks.find({x, z - 1}) != chunks.end())
                updateSide(front, *chunks[{x, z - 1}]);

        } else
            it++;
    }

    for (int i = -radius + chunkx; i <= radius + chunkx; i++)
        for (int j = -radius + chunkz; j <= radius + chunkz; j++)
            if (chunks.find({i, j}) == chunks.end())
                loadChunk(Chunk::generateChunk(i, j));
}
