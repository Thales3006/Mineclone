#ifndef TERRAIN_VERTEX_MAP_CLASS_H
#define TERRAIN_VERTEX_MAP_CLASS_H

#include "render/mesh/mesh.h"
#include "render/textureManager.h"
#include "world/terrain/block/block.h"

#include <map>

class TerrainVertexMap {
  public:
    TerrainVertexMap(std::shared_ptr<TextureManager> textureManager);

    Geometry<TerrainVertex> getGeometry(Block &block);

  private:
    std::map<unsigned char, Geometry<TerrainVertex>> fullBlock;
    std::shared_ptr<TextureManager> textureManager;

    Geometry<TerrainVertex> getUVFace(BlockTextureID id, unsigned char face);
};

#endif
