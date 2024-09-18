#include "playerClass.h"

Player::Player(glm::vec3 pos): Camera(pos+glm::vec3(size.x/2, size.y*0.95, size.z/2), 0.0f, 0.0f), Entity(pos, glm::vec3(0.5, 0.7, 0.5)) {
    size = glm::vec3(0.5, 1.75, 0.5);

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
    
    Camera::position = Entity::position + glm::vec3(size.x/2, size.y*0.95, size.z/2);
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
