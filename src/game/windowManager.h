#ifndef WINDOW_MANAGER_CLASS_H
#define WINDOW_MANAGER_CLASS_H

#include "render/renderLib.h"

#include <glm/vec2.hpp>

class WindowManager {
  public:
    WindowManager();

    GLFWwindow *getWindow();
    GLFWmonitor *getMonitor();

    glm::uvec2 getWindowSize();

    void createWindow(void *game);
    void bindContext();

  private:
    const int defaultWidth = 1200;
    const int defaultHeight = 600;

    GLFWmonitor *monitor;
    GLFWwindow *window;
};

#endif
