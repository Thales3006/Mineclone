#ifndef INPUT_MANAGER_CLASS

#include "game/input/inputMap.h"
#include "render/worldRenderer.h"
#include "world/world.h"

#include <unordered_set>

class InputManager {
  public:
    InputManager();
    InputManager(std::shared_ptr<World> world, Window *window,
                 WorldRenderer *worldRenderer);

    void handleMouseClickCallback(GLFWwindow *window, int button, int action,
                                  int mods);
    void handleMouseMovementCallback(double xoffset, double yoffset);
    void handleKeyboardCallback(GLFWwindow *window, int key, int scancode,
                                int action, int mods);

    void processKeyboard(float deltaTime);

  private:
    InputMap inputMap;
    std::unordered_set<Action> activeActions;

    std::shared_ptr<World> world;
    Window *window;
    WorldRenderer *worldRenderer;

    float sensibility;

    void handleSingleAction(GLFWwindow *window, Action action);
};

#endif
