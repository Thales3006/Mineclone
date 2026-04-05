#ifndef INPUT_MANAGER_CLASS

#include "game/input/inputMap.h"
#include "render/renderManager.h"
#include "world/world.h"

#include <unordered_set>

class InputManager {
  public:
    InputManager();
    InputManager(std::shared_ptr<World> world,
                 std::shared_ptr<WindowManager> windowManager,
                 std::shared_ptr<RenderManager> renderManager);

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
    std::shared_ptr<WindowManager> windowManager;
    std::shared_ptr<RenderManager> renderManager;

    float sensibility;

    void handleSingleAction(GLFWwindow *window, Action action);
};

#endif
