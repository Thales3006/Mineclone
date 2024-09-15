#include "playerClass.h"

Player::Player(glm::vec3 pos): Camera(pos, 0.0f, 0.0f){
    height=1.8;
    width=0.9;

}

void Player::setKeys(int forward, int back, int left, int right, int upward, int downward){
    forward_key = forward;
    back_key = back;
    left_key = left;
    right_key = right;
    up_key = upward;
    down_key = downward;
}

void Player::processKeyMovement(GLFWwindow* window){
    if(glfwGetKey(window, forward_key)){
        velocity += glm::normalize(glm::vec3(direction.x, 0, direction.z)) * acceleration;
    }
    if (glfwGetKey(window, back_key)){
        velocity -= glm::normalize(glm::vec3(direction.x, 0, direction.z)) * acceleration;
    }
    if (glfwGetKey(window, left_key)){
        velocity += glm::normalize(glm::cross(up,direction)) * acceleration;
    }
    if (glfwGetKey(window, right_key)){
        velocity -= glm::normalize(glm::cross(up,direction)) * acceleration;  
    }
    if (glfwGetKey(window, up_key)){
        velocity += up * acceleration;
    }
    if (glfwGetKey(window, down_key)){
        velocity -= up * acceleration;
    }
    velocity.x = velocity.x >= MAX_VELOCITY? MAX_VELOCITY : velocity.x;
    velocity.y = velocity.y >= MAX_VELOCITY? MAX_VELOCITY : velocity.y;
    velocity.z = velocity.z >= MAX_VELOCITY? MAX_VELOCITY : velocity.z;
}

void Player::processMouseMovement(double xoffset, double yoffset){
    xoffset *= getSensitivity();
    yoffset *= getSensitivity();

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > M_PI/2-0.000001)
        pitch = M_PI/2-0.000001;
    if (pitch < -M_PI/2+0.000001)
        pitch = -M_PI/2+0.000001;

    setDirection(yaw, pitch);
}
/*
bool colision(glm::vec3 player, glm::vec3 block){
    return 
    player.x < block.x + block.width && player.x + player.width > block.x && 
    player.y < block.y + block.height && player.y + player.height > block.y &&
    player.z < block.z + block.width && player.z + player.width > block.z; 
}*/

bool colisionBlock(glm::vec3 player, glm::vec3 block){
    return 
    player.x <= block.x + 1.0 && player.x + 0.7 >= block.x && 
    player.y <= block.y + 1.0 && player.y + 1.8 >= block.y &&
    player.z <= block.z + 1.0 && player.z + 0.7 >= block.z; 
}


void Player::update(std::vector<Block> blocks){
    position += velocity;

    for(Block block : blocks){
        if(colisionBlock(position, block.position))
            position -= velocity;
    }
    velocity -= velocity / glm::vec3(MAX_VELOCITY * 2);

}
