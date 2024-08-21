#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "textureClass.h"
#include "cameraClass.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

#define WINDOW_SIZE 600

float windowHeight = WINDOW_SIZE, windowWidth = WINDOW_SIZE;

void drawingScene(GLFWwindow* window, Shader shader, Camera* camera, int VAO, int VBO, int EBO);
void cameraDir(GLFWwindow* window, Camera* camera);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int cubePositions[] = {
	0,0,0
};


Camera camera = Camera(glm::vec3(0.0f,0.0f, 0.0f));

int main(){
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW.\n";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Mineclone", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	if (!gladLoadGL()){
		std::cout << "Failed to initialize Glad.\n";
		return -1;
	}
	glViewport(0, 0, windowWidth, windowHeight);


	Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	glEnable(GL_DEPTH_TEST);

	shader.use();
	shader.setInt("tex0",0);

	camera.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
	glfwSetCursorPosCallback(window, mouseCallback); 

	while(!glfwWindowShouldClose(window)){

		camera.cameraMovement(window);

		drawingScene(window, shader, &camera, VAO, VBO, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void drawingScene(GLFWwindow* window, Shader shader, Camera* camera, int VAO, int VBO, int EBO){
	
	glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	Texture caixa = Texture("textures/container.jpg");
	Texture letra = Texture("textures/a.png");
	Texture bloco = Texture("textures/blocks_01.png");


	glActiveTexture(GL_TEXTURE0);
	bloco.bind();

	glBindVertexArray(VAO);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), windowWidth/windowHeight, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);
	view = camera->viewMat();
	shader.setMat4("view", view);

	for(int i=0; i<sizeof(cubePositions)/sizeof(int); i+=3){

		glm::mat4 model= glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(cubePositions[i+0],cubePositions[i+1],cubePositions[i+2]));
		shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){
	static double xlast = windowWidth/2, ylast = windowHeight/2;
	camera.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}
