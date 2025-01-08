#include "camera.h"

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera() {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);

    sensitivity = 0.005f;
    FOV = M_PI/4;

    minDist = 0.1f;
    maxDist = 100.0f;
    
    setDirection(0, 0);
}

Camera::Camera(glm::vec3 pos, float newYaw, float newPitch) {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = pos;

    sensitivity = 0.005f;
    FOV = M_PI/4;
    
    setDirection(newYaw, newPitch);
}

glm::mat4 Camera::getMatrixView(){
    return glm::lookAt(position, position+direction, up);
}

glm::mat4 Camera::getMatrixProjection(float aspect){
    return glm::perspective(FOV, aspect, minDist, maxDist);
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
    FOV = FOV > 0.1f? FOV : 0.1f;
}

glm::vec3 Camera::getDirection(){
    return direction;
}

glm::vec3 Camera::getUp(){
    return up;
}

float Camera::getSensitivity(){
    return sensitivity;
}

float Camera::getFOV(){
    return FOV;
}

float Camera::getMin(){
    return minDist;
}

float Camera::getMax(){
    return maxDist;
}

void Camera::setMinMax(float newMin,float newMax){
    if(newMin <= 0 || newMax <= 0 || newMin >= newMax)
        return;
    minDist = newMin;
    maxDist = newMax;
}