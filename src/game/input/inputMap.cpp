#include "game/input/inputMap.h"

InputMap::InputMap() {
    setInput(MoveFoward, GLFW_KEY_W);
    setInput(MoveBackward, GLFW_KEY_S);
    setInput(MoveLeft, GLFW_KEY_A);
    setInput(MoveRight, GLFW_KEY_D);
    setInput(Jump, GLFW_KEY_SPACE);
    setInput(DestroyBlock, GLFW_MOUSE_BUTTON_LEFT);
    setInput(PlaceBlock, GLFW_MOUSE_BUTTON_RIGHT);
    setInput(Fullscreen, GLFW_KEY_F11);
    setInput(IncreaseFOV, GLFW_KEY_Z);
    setInput(DecreaseFOV, GLFW_KEY_X);

    setInput(HotbarSlot1, GLFW_KEY_1);
    setInput(HotbarSlot2, GLFW_KEY_2);
    setInput(HotbarSlot3, GLFW_KEY_3);
    setInput(HotbarSlot4, GLFW_KEY_4);
    setInput(HotbarSlot5, GLFW_KEY_5);
    setInput(HotbarSlot6, GLFW_KEY_6);
    setInput(HotbarSlot7, GLFW_KEY_7);
    setInput(HotbarSlot8, GLFW_KEY_8);
    setInput(HotbarSlot9, GLFW_KEY_9);
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
