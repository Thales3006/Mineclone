#ifndef CHUNK_VIEW_CLASS_H
#define CHUNK_VIEW_CLASS_H

#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/textureManager.h"
#include "world/terrain/chunk/chunk.h"
#include "world/terrain/chunk/chunkRegion.h"

#include <map>
#include <utility>

struct ChunkMeshData {
    int x, z;
    Geometry<TerrainVertex> geometry;
    std::map<int, std::pair<unsigned int, unsigned int>> blockRanges;
};

class ChunkView {
  public:
    ChunkView(ChunkMeshData data, TerrainVertexMap &terrainVertexMap,
              const std::shared_ptr<TextureManager> &textureManager,
              std::shared_ptr<Shader> shader);

    static ChunkMeshData buildMeshData(const ChunkRegion &region,
                                       TerrainVertexMap &terrainVertexMap);

    void render(int chunkx, int chunkz);

    void updateBlock(const ChunkRegion &region, glm::ivec3 worldEdit);

    int x, z;

  private:
    TerrainVertexMap *terrainVertexMap;
    Mesh<TerrainVertex> mesh;
    std::map<int, std::pair<unsigned int, unsigned int>> blockRanges;

    static int blockIndex(int i, int j, int k);
    static Geometry<TerrainVertex> blockGeometry(TerrainVertexMap &terrainVertexMap,
                                                 const ChunkRegion &region,
                                                 int i, int j, int k);
    void respliceBlock(const ChunkRegion &region, int i, int j, int k);
};

#endif
