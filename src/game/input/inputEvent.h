#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <variant>

struct KeyEvent {
    int key, scancode, action, mods;
};
struct MouseButton {
    int button, action, mods;
    double x, y;
};
struct MouseMove {
    double dx, dy;
};
struct Scroll {
    double dx, dy;
};

using InputEvent = std::variant<KeyEvent, MouseButton, MouseMove, Scroll>;

#endif
