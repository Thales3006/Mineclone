#ifndef TERRAIN_VERTEX_MAP_CLASS_H
#define TERRAIN_VERTEX_MAP_CLASS_H

#include "render/mesh/mesh.h"
#include "render/textureManager.h"
#include "world/terrain/block/block.h"
#include "world/terrain/chunk/chunkRegion.h"
#include <map>

class TerrainVertexMap {
  public:
    TerrainVertexMap(std::shared_ptr<TextureManager> textureManager);

    Geometry<TerrainVertex> getGeometry(Block &block, unsigned char faces);

  private:
    std::map<unsigned char, Geometry<TerrainVertex>> fullBlock;
    Geometry<TerrainVertex> plantBlock;

    std::shared_ptr<TextureManager> textureManager;

    Geometry<TerrainVertex> setUV(Geometry<TerrainVertex> geometry,
                                  BlockTextureID id);
};

#endif
