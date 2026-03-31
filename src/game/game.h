#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "opengl.h"
#include "render/mesh.h"
#include "render/shader.h"
#include "render/texture.h"
#include "world/chunkManager.h"
#include "world/entities/player.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <vector>

class Game {
  private:
    GLFWmonitor *monitor;
    GLFWwindow *window;
    int windowSize[2];

    double deltaTime;

    ChunkManager chunkManager;
    Player player;

    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::vector<Shader> shaders;

    void openGLInit();

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseClickCallback(GLFWwindow *window, int button, int action, int mods);
    static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

  public:
    Game();

    void run();
};

#endif // GAME_CLASS_H
