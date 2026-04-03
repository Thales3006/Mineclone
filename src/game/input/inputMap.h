#ifndef INPUT_MAP_CLASS
#define INPUT_MAP_CLASS

#include "render/renderLib.h"
#include <map>

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
    None,
};

class InputMap {
  public:
    InputMap();

    void setInput(Action action, int key);
    Action getInput(int key);

  private:
    std::map<Action, int> actionKeyMap;
    std::map<int, Action> keyActionMap;
};

#endif
