#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "textureClass.h"
#include "cameraClass.h"
#include "meshClass.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

int windowHeight = 600, windowWidth = 800;

void drawingScene(GLFWwindow* window, Shader shader, Camera* camera, Mesh* mesh);
void cameraDir(GLFWwindow* window, Camera* camera);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

Camera camera = Camera(glm::vec3(0.0f,0.0f, 0.0f), 0.0f, 0.0f);

int main(){
    
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW.\n";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Mineclone", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()){
		std::cout << "Failed to initialize Glad.\n";
		return -1;
	}
	glViewport(0, 0, windowWidth, windowHeight);

    std::vector<Vertex> vertices = {
        Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},  //1
        Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //2
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},//3

        Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}, //4
        Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},   //5
        Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  //6
        Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}  //7
    };

    std::vector<unsigned int> indices = {
        0,1,5,
        0,4,5,
        1,2,6,
        1,5,6,
        2,3,7,
        7,6,2,
        3,0,4,
        4,7,3,
        0,3,2,
        0,1,2,
        4,5,6,
        4,7,6
    };

    int cubePositions[] = {
        0,0,0,
        1,0,0,
        2,0,0,
        0,0,1,
        0,0,2,
        0,1,0,
        2,0,2
    };

    std::vector<Texture> texturas = {
        Texture("texture_diffuse", "textures/container.jpg"),
        Texture("texture_diffuse", "textures/a.png"),
        Texture("texture_diffuse", "textures/blocks_01.png")
    };

	Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");

    Mesh mesh = Mesh(vertices, indices, texturas);

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	glEnable(GL_DEPTH_TEST);


	camera.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
	glfwSetCursorPosCallback(window, mouseCallback); 

	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, &windowWidth, &windowHeight); 
		glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.cameraMovement(window);

        shader.use();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), float(windowWidth)/windowHeight, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.viewMat();
        shader.setMat4("view", view);

        for(int i=0; i<sizeof(cubePositions)/sizeof(int); i+=3){

            glm::mat4 model= glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(cubePositions[i+0],cubePositions[i+1],cubePositions[i+2]));
            shader.setMat4("model", model);

            mesh.draw(shader);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){
	static double xlast = windowWidth/2, ylast = windowHeight/2;
	camera.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}
