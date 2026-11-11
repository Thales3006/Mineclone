#ifndef INPUT_MAP_CLASS
#define INPUT_MAP_CLASS

#include "game/input/inputEvent.h"

#include <map>
#include <optional>

enum Action {
    MoveFoward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,

    DestroyBlock,
    PlaceBlock,

    HotbarSlot1,
    HotbarSlot2,
    HotbarSlot3,
    HotbarSlot4,
    HotbarSlot5,
    HotbarSlot6,
    HotbarSlot7,
    HotbarSlot8,
    HotbarSlot9,

    Fullscreen,
    IncreaseFOV,
    DecreaseFOV,
};

class InputMap {
  public:
    InputMap();

    std::optional<Action> action(const InputEvent &e) const;
    std::optional<Action> actionForCode(int keyOrButton) const;
    void bind(Action a, int keyOrButton);

  private:
    std::map<Action, int> actionKeyMap;
    std::map<int, Action> keyActionMap;
};

#endif
