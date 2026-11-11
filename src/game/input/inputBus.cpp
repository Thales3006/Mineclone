#include "game/input/inputBus.h"

#include "render/renderLib.h"

InputBus::InputBus(InputMap &map) : map(map) {}

InputEvent InputBus::feedKey(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        keysDown.insert(key);
    else if (action == GLFW_RELEASE)
        keysDown.erase(key);
    return KeyEvent{key, scancode, action, mods};
}

InputEvent InputBus::feedButton(int button, int action, int mods, double x,
                                double y) {
    if (action == GLFW_PRESS)
        keysDown.insert(button);
    else if (action == GLFW_RELEASE)
        keysDown.erase(button);
    return MouseButton{button, action, mods, x, y};
}

InputEvent InputBus::feedMouseMove(double dx, double dy) {
    return MouseMove{dx, dy};
}

InputEvent InputBus::feedScroll(double dx, double dy) { return Scroll{dx, dy}; }

std::optional<Action> InputBus::actionFor(const InputEvent &e) const {
    return map.action(e);
}

bool InputBus::isActionActive(Action a) const {
    for (int code : keysDown)
        if (map.actionForCode(code) == a)
            return true;
    return false;
}
