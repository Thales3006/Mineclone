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
    glm::ivec2 coord;
    Geometry<TerrainVertex> geometry;
    std::map<int, std::pair<unsigned int, unsigned int>> blockRanges;
};

class ChunkView {
  public:
    ChunkView(ChunkMeshData data, TerrainVertexMap &terrainVertexMap,
              const std::shared_ptr<TextureManager> &textureManager,
              std::shared_ptr<Shader> shader);

    static ChunkMeshData buildMeshData(const RegionSnapshot &region,
                                       TerrainVertexMap &terrainVertexMap);

    void render(glm::ivec2 chunk);

    void updateBlock(const RegionSnapshot &region, glm::ivec3 worldEdit);

    glm::ivec2 coord;

  private:
    TerrainVertexMap *terrainVertexMap;
    Mesh<TerrainVertex> mesh;
    std::map<int, std::pair<unsigned int, unsigned int>> blockRanges;

    static int blockIndex(glm::ivec3 pos);
    static Geometry<TerrainVertex> blockGeometry(TerrainVertexMap &terrainVertexMap,
                                                 const RegionSnapshot &region,
                                                 glm::ivec3 pos);
    void respliceBlock(const RegionSnapshot &region, glm::ivec3 pos);
};

#endif
