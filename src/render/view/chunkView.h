#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/textureManager.h"
#include "world/terrain/chunk/chunk.h"
#include "world/terrain/chunk/chunkRegion.h"

#include <optional>

class ChunkView {
  public:
    ChunkView(ChunkRegion region, TerrainVertexMap &terrainVertexMap,
              std::shared_ptr<TextureManager> textureManager,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);

    ChunkRegion region;

    Mesh<TerrainVertex>
    generateChunkMesh(ChunkRegion region, TerrainVertexMap &terrainVertexMap,
                      std::shared_ptr<TextureManager> textureManager,
                      std::shared_ptr<Shader> shader);

  private:
    Mesh<TerrainVertex> mesh;
};

#endif
