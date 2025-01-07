#include "chunkManager.h"

#include <cmath>
#include <iostream>

ChunkManager::ChunkManager() {}

void ChunkManager::loadChunk(Chunk chunk) {
    if(chunks.find({chunk.x, chunk.z}) != chunks.end())
        return;
    chunks[{chunk.x, chunk.z}] = chunk;

    if(chunks.find({chunk.x-1,chunk.z}) != chunks.end())
        updateChunk(chunks[{chunk.x-1,chunk.z}]);
    if(chunks.find({chunk.x+1,chunk.z}) != chunks.end())
        updateChunk(chunks[{chunk.x+1,chunk.z}]);
    if(chunks.find({chunk.x,chunk.z+1}) != chunks.end())
        updateChunk(chunks[{chunk.x,chunk.z+1}]);
    if(chunks.find({chunk.x,chunk.z-1}) != chunks.end())
        updateChunk(chunks[{chunk.x,chunk.z-1}]);
}

void ChunkManager::unloadChunk(int x, int z) {
    if(chunks.find({x, z}) == chunks.end())
        return;
    chunks.erase({x, z});

    if(chunks.find({x-1,z}) != chunks.end())
        updateChunk(chunks[{x-1,z}]);
    if(chunks.find({x+1,z}) != chunks.end())
        updateChunk(chunks[{x+1,z}]);
    if(chunks.find({x,z+1}) != chunks.end())
        updateChunk(chunks[{x,z+1}]);
    if(chunks.find({x,z-1}) != chunks.end())
        updateChunk(chunks[{x,z-1}]);
}

void ChunkManager::updateChunk(Chunk& chunk) {
    auto* leftChunk =  chunks.find({chunk.x-1,chunk.z}) != chunks.end()? &chunks[{chunk.x-1,chunk.z}] : NULL;
    auto* rightChunk = chunks.find({chunk.x+1,chunk.z}) != chunks.end()? &chunks[{chunk.x+1,chunk.z}] : NULL;
    auto* frontChunk = chunks.find({chunk.x,chunk.z+1}) != chunks.end()? &chunks[{chunk.x,chunk.z+1}] : NULL;
    auto* backChunk =  chunks.find({chunk.x,chunk.z-1}) != chunks.end()? &chunks[{chunk.x,chunk.z-1}] : NULL;
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

void ChunkManager::renderChunks(Shader &shader, int chunkx, int chunkz){
    for(auto& [coord, chunk] : chunks)
        chunk.renderChunk(shader,chunkx, chunkz);
}