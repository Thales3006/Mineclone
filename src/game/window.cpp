#include "game/window.h"

#include "game/game.h"

#include <iostream>

Window::Window() : window(nullptr), monitor(nullptr) {
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

    window =
        glfwCreateWindow(defaultWidth, defaultHeight, "Mineclone", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window.\n";
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, static_cast<double>(defaultWidth) / 2,
                     static_cast<double>(defaultHeight) / 2);

    glfwSetCursorPosCallback(window, Game::mouseMoveCallback);
    glfwSetMouseButtonCallback(window, Game::mouseClickCallback);
    glfwSetKeyCallback(window, Game::keyboardCallback);
}

void Window::setUserPointer(void *game) {
    glfwSetWindowUserPointer(window, game);
}

GLFWwindow *Window::getWindow() { return window; }
GLFWmonitor *Window::getMonitor() { return monitor; }

glm::uvec2 Window::getWindowSize() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    return glm::ivec2(windowWidth, windowHeight);
}
