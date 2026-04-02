#include "game/windowManager.h"

#include "game/game.h"

#include <iostream>

void WindowManager::createWindow(void *game) {
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

    window = glfwCreateWindow(defaultWidth, defaultHeight, "Mineclone", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window.\n";
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, game);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, static_cast<double>(defaultWidth) / 2,
                     static_cast<double>(defaultHeight) / 2);

    glfwSetCursorPosCallback(window, Game::mouseMoveCallback);
    glfwSetMouseButtonCallback(window, Game::mouseClickCallback);
    glfwSetKeyCallback(window, Game::keyboardCallback);
}

GLFWwindow *WindowManager::getWindow() { return window; }
GLFWmonitor *WindowManager::getMonitor() { return monitor; }

glm::uvec2 WindowManager::getWindowSize() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    return glm::ivec2(windowWidth, windowHeight);
}
