#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "world/terrain/chunk/chunk.h"

class ChunkMesh {
  public:
    ChunkMesh(Chunk &chunk, std::vector<std::shared_ptr<Texture>> textures,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);
    Mesh<TerrainVertex> mesh;

  private:
    int x, z;
};

#endif
