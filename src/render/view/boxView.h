#ifndef BOX_VIEW_CLASS_H
#define BOX_VIEW_CLASS_H

#include "render/mesh/mesh.h"

class BoxView {
  public:
    void render(int chunkx, int chunkz);

  private:
    Mesh<TerrainVertex> mesh;
};

#endif
