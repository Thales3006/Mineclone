#include "player.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player(glm::vec3 pos, glm::vec3 siz) : 
    Entity(pos, siz), 
    camera(pos+glm::vec3(siz.x/2, siz.y*0.9, siz.z/2), 0.0f, 0.0f)
{
    forward_key = GLFW_KEY_UP;
    back_key = GLFW_KEY_DOWN;
    left_key = GLFW_KEY_LEFT;
    right_key = GLFW_KEY_RIGHT;
    up_key = GLFW_KEY_RIGHT_SHIFT;
    down_key = GLFW_KEY_RIGHT_CONTROL;
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
    glm::vec3 front = glm::normalize(glm::vec3(camera.getDirection().x, 0, camera.getDirection().z));
    glm::vec3 side = glm::normalize(glm::cross(camera.getUp(), camera.getDirection()));
    glm::vec3 up = glm::normalize(camera.getUp());

    if(glfwGetKey(window, forward_key)){
        velocity += front * acceleration;
    }
    if (glfwGetKey(window, back_key)){
        velocity -= front * acceleration;
    }
    if (glfwGetKey(window, left_key)){
        velocity += side * acceleration;
    }
    if (glfwGetKey(window, right_key)){
        velocity -= side * acceleration;  
    }
    if (/*onGround && */glfwGetKey(window, up_key)){
        velocity = up * acceleration;
        //onGround= false;
    }
    if (glfwGetKey(window, down_key)){
        velocity = -up * acceleration;
    }
}

void Player::processMouseMovement(double xoffset, double yoffset){
    xoffset *= camera.getSensitivity();
    yoffset *= camera.getSensitivity();

    camera.yaw   += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > M_PI/2-0.000001)
        camera.pitch = M_PI/2-0.000001;
    else if (camera.pitch < -M_PI/2+0.000001)
        camera.pitch = -M_PI/2+0.000001;
    camera.setDirection(camera.yaw, camera.pitch);
}

void Player::updatePlayer(GLFWwindow* window, std::vector<Block> blocks){
    processKeyMovement(window);
    update(blocks);
    camera.position = position + glm::vec3(size.x/2, size.y*0.9, size.z/2);
}