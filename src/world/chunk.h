#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "block.h"
#include "render/mesh.h"

#include <map>
#include <memory>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 32

enum Side { front, back, left, right };

class Chunk {
  public:
    int x;
    int z;
    Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

    std::unique_ptr<Mesh> mesh;

    Chunk();
    Chunk(int x, int z);
    Chunk(int x, int z, Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH]);

    ~Chunk();

    void setBlock(int x, int y, int z, Block block);
    void updateBlock(int x, int y, int z, const Chunk *leftChunk, const Chunk *rightChunk,
                     const Chunk *frontChunk, const Chunk *backChunk);
    void updateBlocks(const Chunk *leftChunk, const Chunk *rightChunk, const Chunk *frontChunk,
                      const Chunk *backChunk);
    void updateSide(const Side side, const Chunk *leftChunk, const Chunk *rightChunk,
                    const Chunk *frontChunk, const Chunk *backChunk);

    void setMesh();
    void renderChunk(Shader &shader, int chunkx, int chunkz);

    static std::unique_ptr<Chunk> generateChunk(int x, int z);

    static glm::vec3 chunkOffSet(glm::vec3 position);
};

#endif
