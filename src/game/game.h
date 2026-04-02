#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "game/inputMap.h"
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

    void processKeyMovement(int key);
    void processMouseMovement(double xoffset, double yoffset);

  private:
    double deltaTime;
    std::atomic<bool> isRunning;
    float sensibility;

    WindowManager windowManager;
    RenderManager renderManager;
    InputMap inputMap;

    World world;
    void autoLoadChunks();
};

#endif // GAME_CLASS_H
