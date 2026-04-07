#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/textureManager.h"
#include "world/terrain/chunk/chunk.h"

class ChunkView {
  public:
    ChunkView(std::shared_ptr<Chunk> chunk, TerrainVertexMap &terrainVertexMap,
              std::shared_ptr<TextureManager> textureManager,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);

    std::shared_ptr<Chunk> chunk;

  private:
    Mesh<TerrainVertex> mesh;
};

#endif
