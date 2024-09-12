#include "cameraClass.h"

#include <math.h>

Camera::Camera(glm::vec3 pos, float newYaw, float newPitch){
    position = pos;
    setDirection(newYaw, newPitch);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getMatrixView(){
    return glm::lookAt(position, position+direction, up);
}
glm::mat4 Camera::getMatrixProjection(float aspect){
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void Camera::setKeys(int forward, int back, int left, int right, int upward, int downward){
    forward_key = forward;
    back_key = back;
    left_key = left;
    right_key = right;
    up_key = upward;
    down_key = downward;
}

void Camera::processKeyMovement(GLFWwindow* window){
    if(glfwGetKey(window, forward_key)){
        position += glm::normalize(glm::vec3(direction.x, 0, direction.z))*0.1f;
    }
    if (glfwGetKey(window, back_key)){
        position -= glm::normalize(glm::vec3(direction.x, 0, direction.z))*0.1f;
    }
    if (glfwGetKey(window, left_key)){
        position += glm::normalize(glm::cross(up,direction))*0.1f;
    }
    if (glfwGetKey(window, right_key)){
        position -= glm::normalize(glm::cross(up,direction))*0.1f;  
    }
    if (glfwGetKey(window, up_key)){
        position += up*0.1f;
    }
    if (glfwGetKey(window, down_key)){
        position -= up*0.1f;
    }

}

void Camera::processMouseMovement(double xoffset, double yoffset){
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > M_PI/2)
        pitch = M_PI/2;
    if (pitch < -M_PI/2)
        pitch = -M_PI/2;

    setDirection(yaw, pitch);
}

void Camera::setDirection(float newYaw, float newPitch){
    yaw = newYaw;
    pitch = newPitch;
    direction = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
}

void Camera::setSensitivity(float newSensitivity){
    sensitivity = newSensitivity;
}

void Camera::setFOV(float newFOV){
    FOV = newFOV <= M_PI? newFOV : M_PI;
    FOV = FOV > 0? FOV : 0.1f;
}

glm::vec3 Camera::getDirection(){
    return direction;
}

float Camera::getSensitivity(){
    return sensitivity;
}

float Camera::getFOV(){
    return FOV;
}