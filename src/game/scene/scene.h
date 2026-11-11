#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

class Renderer;
struct GLFWwindow;

class Scene {
  public:
    virtual ~Scene() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update(float dt) = 0;
    virtual void render(Renderer &renderer) = 0;

    virtual void onMouseMove(double dx, double dy) {}
    virtual void onMouseButton(GLFWwindow *window, int button, int action,
                               int mods) {}
    virtual void onKey(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {}
};

#endif
