#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "game/input/inputEvent.h"

class Renderer;

class Scene {
  public:
    virtual ~Scene() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update(float dt) = 0;
    virtual void render(Renderer &renderer) = 0;

    virtual void onEvent(const InputEvent &e) {}
};

#endif
