#include "game.h"

Game::Game() {
    windowSize[0] = 1200; windowSize[1] = 600;
    window = openGLInit(windowSize[0], windowSize[1]);
    if(window == NULL) exit(-1);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, Game::mouseCallback);
    
    glfwSetCursorPos(window, windowSize[0]/2, windowSize[1]/2);
    
    textures = {
        Texture("texture_diffuse", "textures/container.jpg"),
        Texture("texture_diffuse", "textures/blocks_01.png")
    };
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
    meshes.push_back(Mesh(vertices, textures));
    shaders.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));

    blocks = {
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
    
    player = Player(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.5, 1.75, 0.5));
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
}

void Game::run() {
    
	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, (windowSize), (windowSize+1)); 
		glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(window, blocks);

        shaders[0].setMat4("projection", player.camera.getMatrixProjection(float(windowSize[0])/windowSize[1]));
        shaders[0].setMat4("view", player.camera.getMatrixView());

        for(auto& block : blocks){
            glm::mat4 model = glm::translate(glm::mat4(1.0f), block.position);
            shaders[0].setMat4("model", model);

            meshes[0].draw(shaders[0]);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    
}

void Game::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	static double xlast = game->windowSize[0]/2, ylast = game->windowSize[1]/2;
	game->player.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}