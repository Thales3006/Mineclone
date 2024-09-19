#include "playerClass.h"

Player::Player(glm::vec3 pos, glm::vec3 siz): Camera(pos+glm::vec3(siz.x/2, siz.y*0.9, siz.z/2), 0.0f, 0.0f), Entity(pos, siz) {

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
    if (onGround && glfwGetKey(window, up_key)){
        velocity = up * acceleration;
        onGround= false;
    }
    //if (glfwGetKey(window, down_key)){
    //    velocity -= up * acceleration;
    //}
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

void Player::updatePlayer(GLFWwindow* window, std::vector<Block> blocks){
    processKeyMovement(window);
    onGround = update(blocks);

    Camera::position = Entity::position + glm::vec3(size.x/2, size.y*0.9, size.z/2);
}