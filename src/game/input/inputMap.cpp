#include "game/input/inputMap.h"

#include "render/renderLib.h"

InputMap::InputMap() {
    bind(MoveFoward, GLFW_KEY_W);
    bind(MoveBackward, GLFW_KEY_S);
    bind(MoveLeft, GLFW_KEY_A);
    bind(MoveRight, GLFW_KEY_D);
    bind(Jump, GLFW_KEY_SPACE);
    bind(DestroyBlock, GLFW_MOUSE_BUTTON_LEFT);
    bind(PlaceBlock, GLFW_MOUSE_BUTTON_RIGHT);
    bind(Fullscreen, GLFW_KEY_F11);
    bind(IncreaseFOV, GLFW_KEY_Z);
    bind(DecreaseFOV, GLFW_KEY_X);

    bind(HotbarSlot1, GLFW_KEY_1);
    bind(HotbarSlot2, GLFW_KEY_2);
    bind(HotbarSlot3, GLFW_KEY_3);
    bind(HotbarSlot4, GLFW_KEY_4);
    bind(HotbarSlot5, GLFW_KEY_5);
    bind(HotbarSlot6, GLFW_KEY_6);
    bind(HotbarSlot7, GLFW_KEY_7);
    bind(HotbarSlot8, GLFW_KEY_8);
    bind(HotbarSlot9, GLFW_KEY_9);
}

void InputMap::bind(Action a, int keyOrButton) {
    auto previous = actionKeyMap.find(a);
    if (previous != actionKeyMap.end())
        keyActionMap.erase(previous->second);

    actionKeyMap[a] = keyOrButton;
    keyActionMap[keyOrButton] = a;
}

std::optional<Action> InputMap::actionForCode(int keyOrButton) const {
    auto it = keyActionMap.find(keyOrButton);
    if (it == keyActionMap.end())
        return std::nullopt;
    return it->second;
}

std::optional<Action> InputMap::action(const InputEvent &e) const {
    if (auto k = std::get_if<KeyEvent>(&e))
        return actionForCode(k->key);
    if (auto b = std::get_if<MouseButton>(&e))
        return actionForCode(b->button);
    return std::nullopt;
}
