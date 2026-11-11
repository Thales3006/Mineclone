#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "game/input/inputBus.h"
#include "game/input/inputMap.h"
#include "game/scene/scene.h"
#include "game/window.h"
#include "render/renderer.h"

#include <memory>

class Game {
  public:
    Game();

    void run();

    void onEvent(const InputEvent &e);

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseClickCallback(GLFWwindow *window, int button, int action,
                                   int mods);
    static void keyboardCallback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods);

  private:
    double deltaTime;

    Window window;
    Renderer renderer;
    InputMap inputMap;
    InputBus bus;
    std::unique_ptr<Scene> scene;
};

#endif // GAME_CLASS_H
