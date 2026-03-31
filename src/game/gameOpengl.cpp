#include "game/game.h"
#include "game/opengl.h"

#include <iostream>

void Game::openGLInit() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW.\n";
        return;
    }

    monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cout << "Failed to get monitor.\n";
        exit(-1);
    }
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode) {
        std::cout << "Failed to get video mode.\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(windowSize[0], windowSize[1], "Mineclone", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window.\n";
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGL()) {
        std::cout << "Failed to initialize Glad.\n";
        return;
    }
    glViewport(0, 0, windowSize[0], windowSize[1]);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, static_cast<double>(windowSize[0]) / 2,
                     static_cast<double>(windowSize[1]) / 2);

    glfwSetCursorPosCallback(window, Game::mouseMoveCallback);
    glfwSetMouseButtonCallback(window, Game::mouseClickCallback);
    glfwSetKeyCallback(window, Game::keyboardCallback);
}
