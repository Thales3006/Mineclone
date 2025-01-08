#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "block.h"

#include "mesh.h"
#include <map>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 32

class Chunk {
    public:
        int x;
        int z;
        Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

        std::map<unsigned int, Mesh> meshes;

        Chunk();
        Chunk(int x, int z);
        Chunk(int x, int z, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]);

        ~Chunk();

        void setBlock(int x, int y, int z, Block block);
        void updateBlock(int x, int y, int z, const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk);
        void updateBlocks(const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk);
 
        static Chunk generateChunk(int x, int z);

        void setMeshes();
        void renderChunk(Shader &shader, int chunkx, int chunkz);
}; 

#endif