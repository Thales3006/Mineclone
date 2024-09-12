#include "playerClass.h"

Player::Player(glm::vec3 pos): Camera(pos, 0.0f, 0.0f){

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

    if (pitch > M_PI/2)
        pitch = M_PI/2;
    if (pitch < -M_PI/2)
        pitch = -M_PI/2;

    setDirection(yaw, pitch);
}

void Player::update(){
    position += velocity;
    velocity -= glm::vec3(velocity.x/MAX_VELOCITY,velocity.y/MAX_VELOCITY,velocity.z/MAX_VELOCITY);//glm::vec3(0, gravity, 0);
}