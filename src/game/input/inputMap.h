#ifndef INPUT_MAP_CLASS
#define INPUT_MAP_CLASS

#include <map>

enum Action {
    MoveFoward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
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
