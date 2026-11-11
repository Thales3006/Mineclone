#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include "render/renderLib.h"

#include <glm/vec2.hpp>

class Window {
  public:
    Window();

    GLFWwindow *getWindow();
    GLFWmonitor *getMonitor();

    glm::uvec2 getWindowSize();

    void setUserPointer(void *game);
    void bindContext();

    void toggleFullscreen();

  private:
    const int defaultWidth = 1200;
    const int defaultHeight = 600;

    bool fullscreen = false;

    GLFWmonitor *monitor;
    GLFWwindow *window;
};

#endif
