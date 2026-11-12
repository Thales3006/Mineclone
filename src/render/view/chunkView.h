#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/textureManager.h"
#include "world/terrain/chunk/chunk.h"
#include "world/terrain/chunk/chunkRegion.h"

#include <map>
#include <utility>

class ChunkView {
  public:
    ChunkView(ChunkRegion region, TerrainVertexMap &terrainVertexMap,
              std::shared_ptr<TextureManager> textureManager,
              std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);

    void updateBlock(ChunkRegion region, glm::ivec3 worldEdit);

    ChunkRegion region;

  private:
    TerrainVertexMap *terrainVertexMap;
    Mesh<TerrainVertex> mesh;
    std::map<int, std::pair<unsigned int, unsigned int>> blockRanges;

    int blockIndex(int i, int j, int k) const;
    Geometry<TerrainVertex> blockGeometry(int i, int j, int k);
    void buildMesh();
    void respliceBlock(int i, int j, int k);
};

#endif
