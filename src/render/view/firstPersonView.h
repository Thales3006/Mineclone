#ifndef FIRST_PERSON_VIEW_CLASS_H
#define FIRST_PERSON_VIEW_CLASS_H

#include "render/mesh/mesh.h"

#include "render/mesh/terrainVertexMap.h"
#include "world/entity/player.h"
#include "world/world.h"

class FirstPersonView {
  public:
    FirstPersonView(std::shared_ptr<Player> player,
                    std::shared_ptr<World> world,
                    TerrainVertexMap &terrainVertexMap,
                    std::shared_ptr<Texture> texture,
                    std::shared_ptr<Shader> shader);

    void render(int chunkx, int chunkz);

  private:
    Mesh<TerrainVertex> selectionBoxMesh;

    std::shared_ptr<Player> player;
    std::shared_ptr<World> world;
};

#endif
