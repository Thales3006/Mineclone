#include "opengl.h"

GLFWwindow* openGLInit(int windowWidth, int windowHeight){
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW.\n";
		return NULL;
	}
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Mineclone", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()){
		std::cout << "Failed to initialize Glad.\n";
		return NULL;
	}
	glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable( GL_CULL_FACE);

    return window;
}