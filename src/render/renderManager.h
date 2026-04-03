#ifndef RENDER_MANAGER_CLASS_H
#define RENDER_MANAGER_CLASS_H

#include "game/windowManager.h"
#include "render/camera.h"
#include "render/shader.h"
#include "render/texture.h"
#include "world/world.h"

class RenderManager {
  public:
    RenderManager();
    RenderManager(WindowManager *WindowManager, World *world);

    void initOpenGL();
    void renderFrame();

    void handleMouseMovement(double xoffset, double yoffset);

    Camera camera;

  private:
    std::vector<Texture> textures;
    std::vector<Shader> shaders;

    WindowManager *windowManager;
    World *world;
};

#endif
