#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "world/terrain/chunk/chunk.h"

class ChunkView {
  public:
    ChunkView(std::shared_ptr<Chunk> chunk,
              std::vector<std::shared_ptr<Texture>> textures,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);
    Mesh<TerrainVertex> mesh;

    std::shared_ptr<Chunk> chunk;
};

#endif
