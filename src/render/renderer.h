#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "game/window.h"

class Renderer {
  public:
    Renderer(Window &window);

    void beginFrame();

    glm::uvec2 getViewportSize();

  private:
    Window &window;
    glm::uvec2 viewportSize;

    void initOpenGL();
};

#endif
