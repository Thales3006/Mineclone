#include "game.h"

Game::Game() {
    windowSize[0] = 1200; windowSize[1] = 600;
    openGLInit();

    

    
    textures = {
        Texture("texture_diffuse", "textures/container.jpg"),
        Texture("texture_diffuse", "textures/blocks_01.png")
    };

    shaders.push_back(Shader("shaders/shader.vert", "shaders/shader.frag"));

    chunkManager.fillChunkRadius(2, player.chunkx, player.chunkz);

    player = Player(glm::vec3(5.0f, 20.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5));
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
    player.camera.setMinMax(0.01f, 400.0f);
    player.camera.setFOV(glm::radians(90.0f));

    deltaTime = 0;
    glfwSetTime(0);
}

void Game::run() {
    
	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, (windowSize), (windowSize+1)); 
		glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //chunkManager.fillChunkRadius(2, player.chunkx, player.chunkz);

        deltaTime = glfwGetTime();
        glfwSetTime(0);
        player.updatePlayer(window, chunkManager.chunks, deltaTime);

        shaders[0].setMat4("projection", player.camera.getMatrixProjection(float(windowSize[0])/windowSize[1]));
        shaders[0].setMat4("view", player.camera.getMatrixView());

        chunkManager.renderChunks(shaders[0], player.chunkx, player.chunkz);

        std::cout << "FPS: "<< 1/deltaTime <<" \n";

        
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}