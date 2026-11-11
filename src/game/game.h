#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "game/scene/scene.h"
#include "game/window.h"
#include "render/renderer.h"

#include <memory>

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

    Window window;
    Renderer renderer;
    std::unique_ptr<Scene> scene;
};

#endif // GAME_CLASS_H
