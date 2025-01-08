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

    shaders.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));

    chunkManager.loadChunk(Chunk::generateChunk(0,0));

    
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
        shaders[0].setMat4("model",glm::mat4());

        chunkManager.fillChunkRadius(2, player.chunkx, player.chunkz);
        chunkManager.renderChunks(shaders[0], player.chunkx, player.chunkz);

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