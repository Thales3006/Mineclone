#include <iostream>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "textureClass.h"
#include "playerClass.h"
#include "meshClass.h"
#include "blockClass.h"
//#include "chunkClass.h"
#include "playerClass.h"
#include "opengl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int windowHeight = 600, windowWidth = 1200;

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

Player player = Player(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.5, 1.75, 0.5));

int main(){

    GLFWwindow* window = openGLInit(windowWidth, windowHeight);
    if(window == NULL) return -1;

    std::vector<Vertex> vertices = {
            //face 1
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},  //1
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},   //5
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},   //5
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}, //4
        //face 2
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  //1
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //2
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //6
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  //1
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //6
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},   //5
        //face 3
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //2
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},//3
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //7
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //2
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //7
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  //6
        //face 4
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},//3
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //0
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //4
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},//3
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //4
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  //7
        //face 5
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},//3
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  //1
        //face 6
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //4
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},   //5
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //6
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //4
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  //6
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}  //7
    };

    std::vector<Block> cubes = {
        Block{glm::vec3(0.0f, 0.0f, 0.0f), 1, true},
        Block{glm::vec3(0.0f, 1.0f, 0.0f), 1, true},
        Block{glm::vec3(1.0f, 0.0f, 0.0f), 1, true},
        Block{glm::vec3(2.0f, 0.0f, 0.0f), 1, true},
        Block{glm::vec3(0.0f, 0.0f, 1.0f), 1, true},
        Block{glm::vec3(0.0f, 0.0f, 2.0f), 1, true},
        Block{glm::vec3(2.0f, 0.0f, 1.0f), 1, true},
        Block{glm::vec3(1.0f, 0.0f, 2.0f), 1, true},
        Block{glm::vec3(1.0f, 0.0f, 1.0f), 1, true},
        Block{glm::vec3(2.0f, 0.0f, 2.0f), 1, true},
    };

    std::vector<Texture> texturas = {
        Texture("texture_diffuse", "textures/blocks_01.png"),
        Texture("texture_diffuse", "textures/blocks_01.png")
    };

	Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");

    Mesh mesh = Mesh(vertices, texturas);

	player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
	glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetCursorPos(window, windowWidth/2, windowHeight/2);

	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, &windowWidth, &windowHeight); 
		glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(window, cubes);

        shader.setMat4("projection", player.getMatrixProjection(float(windowWidth)/windowHeight));
        shader.setMat4("view", player.getMatrixView());

        for(int i=0; i<cubes.size(); i++){

            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubes[i].position);
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
	player.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}
