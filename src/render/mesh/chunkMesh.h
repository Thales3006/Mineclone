#ifndef CHUNK_MESH_CLASS_H
#define CHUNK_MESH_CLASS_H

#include "render/mesh/mesh.h"
#include "world/terrain/chunk/chunk.h"

class ChunkMesh : public Mesh {
  public:
    ChunkMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
              std::vector<std::shared_ptr<Texture>> textures,
              std::shared_ptr<Shader> shader, int x, int z);

    static std::unique_ptr<ChunkMesh>
    fromChunk(Chunk &chunk, std::vector<std::shared_ptr<Texture>> textures,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz) override;

  private:
    int x, z;
};

#endif
