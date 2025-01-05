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
#include "opengl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Game {
private:
    GLFWwindow* window;
    int windowSize[2];

    std::vector<Block> blocks;
    Player player;

    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::vector<Shader> shaders;

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
public:
    Game();

    void run();
};

#endif // GAME_CLASS_H