#include "render/camera.h"

Camera::Camera()
    : Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) {}

Camera::Camera(glm::vec3 position, glm::vec3 direction)
    : position(position), direction(direction), up(0.0f, 1.0f, 0.0f),
      FOV(M_PI / 4), minDist(0.1f), maxDist(100.0f) {}

glm::mat4 Camera::getMatrixView() {
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 Camera::getMatrixProjection(float aspect) {
    return glm::perspective(FOV, aspect, minDist, maxDist);
}

void Camera::setFOV(float newFOV) {
    FOV = newFOV <= M_PI - 0.1 ? newFOV : M_PI - 0.1;
    FOV = FOV >= 0.1f ? FOV : 0.1f;
}

glm::vec3 Camera::getDirection() { return direction; }

glm::vec3 Camera::getUp() { return up; }

float Camera::getFOV() { return FOV; }

float Camera::getMin() { return minDist; }

float Camera::getMax() { return maxDist; }

void Camera::setMinMax(float newMin, float newMax) {
    if (newMin <= 0 || newMax <= 0 || newMin >= newMax)
        return;
    minDist = newMin;
    maxDist = newMax;
}
