#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "game/input/inputManager.h"
#include "game/windowManager.h"
#include "render/renderManager.h"
#include "world/world.h"

#include <atomic>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

class Game {
  public:
    Game();

    void run();

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseClickCallback(GLFWwindow *window, int button, int action,
                                   int mods);
    static void keyboardCallback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods);

  private:
    double deltaTime;
    std::atomic<bool> isRunning;

    std::unique_ptr<InputManager> inputManager;

    std::shared_ptr<WindowManager> windowManager;
    std::shared_ptr<RenderManager> renderManager;

    std::shared_ptr<World> world;
    void autoLoadChunks();
};

#endif // GAME_CLASS_H
