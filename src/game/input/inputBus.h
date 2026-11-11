#ifndef INPUT_BUS_CLASS
#define INPUT_BUS_CLASS

#include "game/input/inputEvent.h"
#include "game/input/inputMap.h"

#include <optional>
#include <unordered_set>

class InputBus {
  public:
    InputBus(InputMap &map);

    InputEvent feedKey(int key, int scancode, int action, int mods);
    InputEvent feedButton(int button, int action, int mods, double x, double y);
    InputEvent feedMouseMove(double dx, double dy);
    InputEvent feedScroll(double dx, double dy);

    std::optional<Action> actionFor(const InputEvent &e) const;
    bool isActionActive(Action a) const;

  private:
    InputMap &map;
    std::unordered_set<int> keysDown;
};

#endif
