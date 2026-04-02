#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "game/windowManager.h"
#include "render/renderManager.h"
#include "world/world.h"

#include <atomic>
#include <cmath>
#include <iostream>
#include <vector>

class Game {
  public:
    Game();

    void run();

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);
    static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

  private:
    double deltaTime;
    std::atomic<bool> isRunning;

    WindowManager windowManager;
    RenderManager renderManager;

    World world;
    void autoLoadChunks();
};

#endif // GAME_CLASS_H
