#include "cameraClass.h"

Camera::Camera(glm::vec3 pos, float newYaw, float newPitch) {
    position = pos;
    setDirection(newYaw, newPitch);
}

glm::mat4 Camera::getMatrixView(){
    return glm::lookAt(position, position+direction, up);
}
glm::mat4 Camera::getMatrixProjection(float aspect){
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
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
    FOV = FOV > 0.1? FOV : 0.1f;
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