#ifndef GAME_VIEW_CLASS_H
#define GAME_VIEW_CLASS_H

#include "render/camera.h"
#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/renderer.h"
#include "render/shader.h"
#include "render/textureManager.h"
#include "render/view/chunkView.h"
#include "render/view/firstPersonView.h"
#include "world/world.h"

class GameView {
  public:
    GameView(World &world, Renderer &renderer);

    void render();

    Camera camera;

  private:
    std::shared_ptr<TextureManager> textureManager;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::shared_ptr<TerrainVertexMap> terrainVertexMap;

    Renderer &renderer;

    std::vector<std::unique_ptr<ChunkView>> chunkViews;
    std::unique_ptr<FirstPersonView> firstPersonView;

    World &world;

    void renderChunks(int chunkx, int chunkz);

    void addChunkView(ChunkMeshData data);
    void removeChunkView(const ChunkRegion &region);
    void updateChunkView(const ChunkRegion &region, glm::ivec3 worldEdit);
};

#endif
