#include "game/input/inputMap.h"

InputMap::InputMap() {
    setInput(MoveFoward, GLFW_KEY_W);
    setInput(MoveBackward, GLFW_KEY_S);
    setInput(MoveLeft, GLFW_KEY_A);
    setInput(MoveRight, GLFW_KEY_D);
    setInput(Jump, GLFW_KEY_SPACE);
    setInput(Fullscreen, GLFW_KEY_F11);
    setInput(IncreaseFOV, GLFW_KEY_Z);
    setInput(DecreaseFOV, GLFW_KEY_X);
}

void InputMap::setInput(Action action, int key) {
    actionKeyMap[action] = key;
    keyActionMap[key] = action;
}

Action InputMap::getInput(int key) {
    auto it = keyActionMap.find(key);

    if (it != keyActionMap.end()) {
        return it->second;
    } else {
        return None;
    }
}
