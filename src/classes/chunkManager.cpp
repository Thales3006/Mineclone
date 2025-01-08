#include "chunkManager.h"

#include <cmath>
#include <iostream>

ChunkManager::ChunkManager() {}

void ChunkManager::loadChunk(Chunk chunk) {
    if(chunks.find({chunk.x, chunk.z}) != chunks.end())
        return;
    
    updateChunk(chunk);
    chunks[{chunk.x, chunk.z}] = chunk;

    if(chunks.find({chunk.x-1,chunk.z}) != chunks.end())
        updateSide(right, chunks[{chunk.x-1,chunk.z}]);
    if(chunks.find({chunk.x+1,chunk.z}) != chunks.end())
        updateSide(left, chunks[{chunk.x+1,chunk.z}]);
    if(chunks.find({chunk.x,chunk.z+1}) != chunks.end())
        updateSide(back, chunks[{chunk.x,chunk.z+1}]);
    if(chunks.find({chunk.x,chunk.z-1}) != chunks.end())
        updateSide(front, chunks[{chunk.x,chunk.z-1}]);

}

void ChunkManager::unloadChunk(int x, int z) {
    if(chunks.find({x, z}) == chunks.end())
        return;

    chunks.erase({x, z});

    if(chunks.find({x-1,z}) != chunks.end())
        updateSide(right, chunks[{x-1,z}]);
    if(chunks.find({x+1,z}) != chunks.end())
        updateSide(left, chunks[{x+1,z}]);
    if(chunks.find({x,z+1}) != chunks.end())
        updateSide(back, chunks[{x,z+1}]);
    if(chunks.find({x,z-1}) != chunks.end())
        updateSide(front, chunks[{x,z-1}]);
}

void ChunkManager::updateSide(const Side side, Chunk& chunk) {
    Chunk* leftChunk =  (chunks.find({chunk.x-1,chunk.z}) != chunks.end())? &chunks[{chunk.x-1,chunk.z}] : NULL;
    Chunk* rightChunk = (chunks.find({chunk.x+1,chunk.z}) != chunks.end())? &chunks[{chunk.x+1,chunk.z}] : NULL;
    Chunk* frontChunk = (chunks.find({chunk.x,chunk.z+1}) != chunks.end())? &chunks[{chunk.x,chunk.z+1}] : NULL;
    Chunk* backChunk =  (chunks.find({chunk.x,chunk.z-1}) != chunks.end())? &chunks[{chunk.x,chunk.z-1}] : NULL;

    chunk.updateSide(side, leftChunk, rightChunk, frontChunk, backChunk);  
}

void ChunkManager::updateChunk(Chunk& chunk) {
    Chunk* leftChunk =  (chunks.find({chunk.x-1,chunk.z}) != chunks.end())? &chunks[{chunk.x-1,chunk.z}] : NULL;
    Chunk* rightChunk = (chunks.find({chunk.x+1,chunk.z}) != chunks.end())? &chunks[{chunk.x+1,chunk.z}] : NULL;
    Chunk* frontChunk = (chunks.find({chunk.x,chunk.z+1}) != chunks.end())? &chunks[{chunk.x,chunk.z+1}] : NULL;
    Chunk* backChunk =  (chunks.find({chunk.x,chunk.z-1}) != chunks.end())? &chunks[{chunk.x,chunk.z-1}] : NULL;
    chunk.updateBlocks(leftChunk, rightChunk, frontChunk, backChunk);
    chunk.setMeshes();
}

void ChunkManager::updateChunks() {
    for (auto& [coords, chunk] : chunks)
        updateChunk(chunk);
}

void ChunkManager::updateBlock(Chunk& chunk, int x, int y, int z){
    auto* leftChunk =  chunks.find({chunk.x-1,chunk.z}) != chunks.end()? &chunks[{chunk.x-1,chunk.z}] : NULL;
    auto* rightChunk = chunks.find({chunk.x+1,chunk.z}) != chunks.end()? &chunks[{chunk.x+1,chunk.z}] : NULL;
    auto* frontChunk = chunks.find({chunk.x,chunk.z+1}) != chunks.end()? &chunks[{chunk.x,chunk.z+1}] : NULL;
    auto* backChunk =  chunks.find({chunk.x,chunk.z-1}) != chunks.end()? &chunks[{chunk.x,chunk.z-1}] : NULL;

    chunk.updateBlock(x,y,z, leftChunk, rightChunk, frontChunk, backChunk);
}

void ChunkManager::updateRegion(Chunk& chunk, int x, int y, int z){
    auto* leftChunk =  chunks.find({chunk.x-1,chunk.z}) != chunks.end()? &chunks[{chunk.x-1,chunk.z}] : NULL;
    auto* rightChunk = chunks.find({chunk.x+1,chunk.z}) != chunks.end()? &chunks[{chunk.x+1,chunk.z}] : NULL;
    auto* frontChunk = chunks.find({chunk.x,chunk.z+1}) != chunks.end()? &chunks[{chunk.x,chunk.z+1}] : NULL;
    auto* backChunk =  chunks.find({chunk.x,chunk.z-1}) != chunks.end()? &chunks[{chunk.x,chunk.z-1}] : NULL;

    chunk.updateBlock(x,y,z, leftChunk, rightChunk, frontChunk, backChunk);

    if(x+1 < CHUNK_WIDTH)
        chunk.updateBlock(x+1,y,z, leftChunk, rightChunk, frontChunk, backChunk);
    else if(rightChunk){
        updateBlock(*rightChunk, 0,y,z);
        rightChunk->setMeshes();
    }
    if(x-1 >= 0)
        chunk.updateBlock(x-1,y,z, leftChunk, rightChunk, frontChunk, backChunk);
    else if(leftChunk){
        updateBlock(*leftChunk, CHUNK_WIDTH-1,y,z);
        leftChunk->setMeshes();
    }

    if(y+1 < CHUNK_WIDTH) 
        chunk.updateBlock(x,y+1,z, leftChunk, rightChunk, frontChunk, backChunk);
    if(y-1 >= 0) 
        chunk.updateBlock(x,y-1,z, leftChunk, rightChunk, frontChunk, backChunk);

    if(z+1 < CHUNK_WIDTH) 
        chunk.updateBlock(x,y,z+1, leftChunk, rightChunk, frontChunk, backChunk);
    else if(frontChunk) {
        updateBlock(*frontChunk, x,y,0);
        frontChunk->setMeshes();
    }
    if(z-1 >= 0) 
        chunk.updateBlock(x,y,z-1, leftChunk, rightChunk, frontChunk, backChunk);
    else if(backChunk){
        updateBlock(*backChunk, x,y, CHUNK_WIDTH-1);
        backChunk->setMeshes();
    }
    chunk.setMeshes();
}

void ChunkManager::setBlock(int chunkx, int chunkz, int x, int y, int z, Block block){
    if((chunks.find({chunkx, chunkz}) == chunks.end()) || (x < 0 || x >= CHUNK_WIDTH) || (y < 0 || y >= CHUNK_HEIGHT) || (z < 0 || z >= CHUNK_WIDTH))
        return;

    Chunk& chunk = chunks[{chunkx, chunkz}];

    chunk.setBlock(x,y,z, block);
    updateRegion(chunk, x,y,z);
}

void ChunkManager::setBlock(int chunkx, int chunkz, glm::vec3 pos, Block block){
    while(pos.x < 0){
        pos +=  glm::vec3(CHUNK_WIDTH, 0, 0);
        chunkx--;
    }
    while(pos.x >= CHUNK_WIDTH){
        pos +=  glm::vec3(-CHUNK_WIDTH, 0, 0);
        chunkx++;
    }
    while(pos.z < 0){
        pos +=  glm::vec3(0, 0, CHUNK_WIDTH);
        chunkz--;
    }
    while(pos.z >= CHUNK_WIDTH){
        pos +=  glm::vec3(0, 0, -CHUNK_WIDTH);
        chunkz++;
    }

    if((chunks.find({chunkx, chunkz}) == chunks.end()))
        return;

    Chunk& chunk = chunks[{chunkx, chunkz}];

    chunk.setBlock(pos.x,pos.y,pos.z, block);
    updateRegion(chunk, pos.x,pos.y,pos.z);
}

void ChunkManager::renderChunks(Shader &shader, int chunkx, int chunkz){
    for(auto& [coord, chunk] : chunks)
        chunk.renderChunk(shader, chunkx,chunkz);
}

void ChunkManager::fillChunkRadius(int radius, int chunkx, int chunkz){
    auto it = chunks.begin();
    while(it != chunks.end()) {
        const auto& [coord, chunk] = *it;
        int x = std::get<0>(coord);
        int z = std::get<1>(coord);
        if (x < -radius+chunkx || x > radius+chunkx || z < -radius+chunkz || z > radius+chunkz){
            it = chunks.erase(it);
            if(chunks.find({x-1,z}) != chunks.end())
                updateSide(right, chunks[{x-1,z}]);
            if(chunks.find({x+1,z}) != chunks.end())
                updateSide(left, chunks[{x+1,z}]);
            if(chunks.find({x,z+1}) != chunks.end())
                updateSide(back, chunks[{x,z+1}]);
            if(chunks.find({x,z-1}) != chunks.end())
                updateSide(front, chunks[{x,z-1}]);

        }
        else
            it++;
    }

    for(int i = -radius + chunkx; i <= radius + chunkx; i++)
        for(int j = -radius + chunkz; j <= radius + chunkz; j++)
            if(chunks.find({i,j}) == chunks.end())
                loadChunk(Chunk::generateChunk(i,j));
}