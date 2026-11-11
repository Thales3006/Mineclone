#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "game/window.h"
#include "render/camera.h"
#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/shader.h"
#include "render/textureManager.h"
#include "render/view/chunkView.h"
#include "render/view/firstPersonView.h"
#include "world/world.h"

#include <queue>

class Renderer {
  public:
    Renderer();
    Renderer(Window *window, std::shared_ptr<World> world);

    void initOpenGL();

    void renderFrame();

    Camera camera;

  private:
    std::shared_ptr<TextureManager> textureManager;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::shared_ptr<TerrainVertexMap> terrainVertexMap;

    std::mutex taskMutex;
    std::queue<std::function<void()>> pendingTasks;

    std::vector<std::unique_ptr<ChunkView>> chunkViews;
    std::unique_ptr<FirstPersonView> firstPersonView;

    Window *window;
    std::shared_ptr<World> world;

    void renderChunks(int chunkx, int chunkz);

    void addChunkView(ChunkRegion region);
    void removeChunkView(ChunkRegion region);
    void updateChunkView(ChunkRegion region);

    void excuteTasks();
};

#endif
