#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <iostream>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"
#include "player.h"
#include "mesh.h"
#include "block.h"
#include "player.h"
#include "chunkManager.h"
#include "opengl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Game {
private:
    GLFWmonitor* monitor;
    GLFWwindow* window;
    int windowSize[2];
    //bool fullScreen;

    double deltaTime;

    ChunkManager chunkManager;
    Player player;

    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::vector<Shader> shaders;

    void openGLInit();

    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    Game();

    void run();
};

#endif // GAME_CLASS_H