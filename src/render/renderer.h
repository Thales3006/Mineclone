#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "game/window.h"

#include <functional>
#include <mutex>
#include <queue>

class Renderer {
  public:
    Renderer(Window &window);

    void beginFrame();

    glm::uvec2 getViewportSize();

    void runOnMain(std::function<void()> task);

  private:
    Window &window;
    glm::uvec2 viewportSize;

    std::mutex taskMutex;
    std::queue<std::function<void()>> pendingTasks;

    void initOpenGL();
    void executeTasks();
};

#endif
