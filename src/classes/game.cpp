#include "game.h"

Game::Game() {
    windowSize[0] = 1200; windowSize[1] = 600;
    window = openGLInit(windowSize[0], windowSize[1]);
    if(window == NULL) exit(-1);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, Game::mouseMoveCallback);
    glfwSetMouseButtonCallback(window, Game::mouseClickCallback);
    
    glfwSetCursorPos(window, windowSize[0]/2, windowSize[1]/2);

    deltaTime = glfwGetTime();
    
    textures = {
        Texture("texture_diffuse", "textures/container.jpg"),
        Texture("texture_diffuse", "textures/blocks_01.png")
    };

    std::vector<Vertex> faceFront = {
        //face FRONT
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}, //1
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)}, //3
    };

    std::vector<Vertex> faceBack = {
        //face BACK
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, //1
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)}, //3
    };

    std::vector<Vertex> faceRight = {
        //face RIGHT
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //1
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}, //2
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //3
    };

    std::vector<Vertex> faceLeft = {
        //face LEFT
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}, //1
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //3
    };

    std::vector<Vertex> faceUp = {
        //face UP
        Vertex{glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //1
        Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}  //3
    };

    std::vector<Vertex> faceDown = {
        //face DOWN
        Vertex{glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //0
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}, //1
        Vertex{glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}, //2
        Vertex{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}, //3
    };

    std::vector<unsigned int> indices = {0,2,1, 0,3,2};

    meshes.push_back(Mesh(faceFront, std::vector<unsigned int>({0,1,3, 1,2,3}), textures));
    meshes.push_back(Mesh(faceBack,  std::vector<unsigned int>({0,3,2, 0,2,1}), textures));
    meshes.push_back(Mesh(faceRight, std::vector<unsigned int>({0,1,3, 1,2,3}), textures));
    meshes.push_back(Mesh(faceLeft,  std::vector<unsigned int>({0,3,2, 0,2,1}), textures));
    meshes.push_back(Mesh(faceUp,    std::vector<unsigned int>({0,1,3, 1,2,3}), textures));
    meshes.push_back(Mesh(faceDown,  std::vector<unsigned int>({0,3,2, 0,2,1}), textures));

    shaders.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            chunkManager.loadChunk(Chunk::generateChunk(i,j));

    
    player = Player(glm::vec3(5.0f, 16.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5));
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
}

void Game::run() {
    
	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, (windowSize), (windowSize+1)); 
		glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(window, chunkManager.chunks);

        shaders[0].setMat4("projection", player.camera.getMatrixProjection(float(windowSize[0])/windowSize[1]));
        shaders[0].setMat4("view", player.camera.getMatrixView());

        for (const auto& [coords, chunk] : chunkManager.chunks)
            for (int x = 0; x < CHUNK_WIDTH; x++)
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                    for (int z = 0; z < CHUNK_WIDTH; z++) {
                        const Block& block = chunk.blocks[x][y][z];
                        if(block.ID == 0 || block.faces == NO_FACE)
                            continue;
                        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z) + glm::vec3(chunk.x-player.chunkx,0,chunk.z-player.chunkz)*glm::vec3(CHUNK_WIDTH));
                        shaders[0].setMat4("model", model);

                        if(block.faces & FRONT_FACE)
                            meshes[0].draw(shaders[0]);
                        if(block.faces & BACK_FACE)
                            meshes[1].draw(shaders[0]);
                        if(block.faces & RIGHT_FACE)
                            meshes[2].draw(shaders[0]);
                        if(block.faces & LEFT_FACE)
                            meshes[3].draw(shaders[0]);
                        if(block.faces & UP_FACE)
                            meshes[4].draw(shaders[0]);
                        if(block.faces & DOWN_FACE)
                            meshes[5].draw(shaders[0]);
                    }

        deltaTime = glfwGetTime();
        glfwSetTime(0);
        std::cout << "FPS: "<< 1/deltaTime <<" \n";

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Game::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	static double xlast = game->windowSize[0]/2, ylast = game->windowSize[1]/2;
	game->player.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow* window, int button, int action, int mods){
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        std::cout << "Mouse Clicked\n";
        game->chunkManager.setBlock(
            game->player.chunkx, 
            game->player.chunkz, 
            int(game->player.camera.position.x), 
            int(game->player.camera.position.y), 
            int(game->player.camera.position.z), 
            Block());
    }
}