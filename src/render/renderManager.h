#ifndef RENDER_MANAGER_CLASS_H
#define RENDER_MANAGER_CLASS_H

#include "game/windowManager.h"
#include "render/camera.h"
#include "render/mesh/mesh.h"
#include "render/mesh/terrainVertexMap.h"
#include "render/shader.h"
#include "render/textureManager.h"
#include "render/view/chunkView.h"
#include "render/view/firstPersonView.h"
#include "world/world.h"

#include <queue>

class RenderManager {
  public:
    RenderManager();
    RenderManager(std::shared_ptr<WindowManager> WindowManager,
                  std::shared_ptr<World> world);

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

    std::shared_ptr<WindowManager> windowManager;
    std::shared_ptr<World> world;

    void renderChunks(int chunkx, int chunkz);

    void addChunkView(std::shared_ptr<Chunk> chunk);
    void removeChunkView(std::shared_ptr<Chunk> chunk);
    void updateChunkView(std::shared_ptr<Chunk> chunk);

    void excuteTasks();
};

#endif
