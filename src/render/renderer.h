#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "game/window.h"

class Renderer {
  public:
    Renderer();
    Renderer(Window *window);

    void beginFrame();

    Window *getWindow();

  private:
    Window *window;

    void initOpenGL();
};

#endif
