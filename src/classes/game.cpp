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

    chunkManager.fillChunkRadius(2, player.chunkx, player.chunkz);

    player = Player(glm::vec3(5.0f, 20.0f, 5.0f), glm::vec3(0.5, 1.75, 0.5));
    player.setKeys(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
    player.camera.setMinMax(0.01f, 400.0f);
}

void Game::run() {
    
	while(!glfwWindowShouldClose(window)){
        glfwGetWindowSize(window, (windowSize), (windowSize+1)); 
		glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bool fullScreen = false;
        if (glfwGetKey(window, GLFW_KEY_F11)){
            if(!fullScreen){
                glfwSetWindowMonitor(window, 0, 0, 0, 1920, 1080,60);
                fullScreen = true;
            }
            else {
                glfwSetWindowMonitor(window, 0, 400, 400, windowSize[0], windowSize[1],60);
                fullScreen = false;
            }
        }

        player.updatePlayer(window, chunkManager.chunks, deltaTime);

        shaders[0].setMat4("projection", player.camera.getMatrixProjection(float(windowSize[0])/windowSize[1]));
        shaders[0].setMat4("view", player.camera.getMatrixView());

        //chunkManager.fillChunkRadius(2, player.chunkx, player.chunkz);
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
    Game& game = *(static_cast<Game*>(glfwGetWindowUserPointer(window)));
	static double xlast = game.windowSize[0]/2, ylast = game.windowSize[1]/2;
	game.player.processMouseMovement(xpos-xlast, ylast-ypos);
	xlast = xpos; ylast = ypos;
}

void Game::mouseClickCallback(GLFWwindow* window, int button, int action, int mods){
    Game& game = *(static_cast<Game*>(glfwGetWindowUserPointer(window)));
    Player& player = game.player;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glm::vec3 dir = glm::normalize(player.camera.getDirection());
        glm::vec3 i = glm::vec3(0);
        while(game.chunkManager.getBlock(player.chunkx, player.chunkz, player.camera.position + dir*i).ID == 0){
            if(i.x>6)
                return;
            i += glm::vec3(1);
        }
        game.chunkManager.setBlock(player.chunkx, player.chunkz, player.camera.position + dir*i, Block());
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glm::vec3 dir = glm::normalize(player.camera.getDirection());
        glm::vec3 i = glm::vec3(0);
        while(game.chunkManager.getBlock(player.chunkx, player.chunkz, player.camera.position + dir*i).ID == 0){
            if(i.x>6)
                return;
            i += glm::vec3(1);
        }

        if(!Entity::colision(player.camera.position + dir*i, glm::vec3(0,0,0), glm::floor(player.position), glm::ceil(player.position+player.size)-glm::floor(player.position)))
            game.chunkManager.setBlock(player.chunkx, player.chunkz, player.camera.position + dir*i, Block(1, true));
    }
}