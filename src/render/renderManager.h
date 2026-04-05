#ifndef RENDER_MANAGER_CLASS_H
#define RENDER_MANAGER_CLASS_H

#include "game/windowManager.h"
#include "render/camera.h"
#include "render/mesh/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "render/view/chunkView.h"
#include "world/world.h"

class RenderManager {
  public:
    RenderManager();
    RenderManager(WindowManager *WindowManager, World *world);

    void initOpenGL();

    void updateView();
    void renderFrame();

    void handleMouseMovement(double xoffset, double yoffset);

    Camera camera;

  private:
    std::map<std::string, std::shared_ptr<Texture>> textures;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::vector<std::unique_ptr<ChunkMesh>> meshes;

    WindowManager *windowManager;
    World *world;

    void renderChunks(int chunkx, int chunkz);
};

#endif
