#include "opengl.h"

#include "game.h"
#include <iostream>

void Game::openGLInit(int windowWidth, int windowHeight){
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW.\n";
		return;
	}
	window = glfwCreateWindow(windowWidth, windowHeight, "Mineclone", NULL, NULL);
	glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

	if (!gladLoadGL()){
		std::cout << "Failed to initialize Glad.\n";
		return;
	}
	glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable( GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, windowSize[0]/2, windowSize[1]/2);
        
    glfwSetCursorPosCallback(window, Game::mouseMoveCallback);
    glfwSetMouseButtonCallback(window, Game::mouseClickCallback);
}