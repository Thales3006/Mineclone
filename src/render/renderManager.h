#ifndef RENDER_MANAGER_CLASS_H
#define RENDER_MANAGER_CLASS_H

#include "game/windowManager.h"
#include "render/camera.h"
#include "render/shader.h"
#include "render/texture.h"
#include "world/entity/player.h"
#include "world/terrain/chunk/chunkManager.h"

class RenderManager {
  public:
    RenderManager();
    RenderManager(WindowManager *windowManager, ChunkManager *chunkManager, Player *player);

    void initOpenGL();
    void renderFrame();

  private:
    std::vector<Texture> textures;
    std::vector<Shader> shaders;

    WindowManager *windowManager;
    ChunkManager *chunkManager;
    Player *player;
};

#endif
