#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  private:
    glm::vec3 up;
    float FOV;

    float minDist;
    float maxDist;

  public:
    glm::vec3 position;
    glm::vec3 direction;

    Camera();
    Camera(glm::vec3 position, glm::vec3 direction);

    glm::mat4 getMatrixView();
    glm::mat4 getMatrixProjection(float aspect);

    void setFOV(float newFOV);
    void setMinMax(float newMin, float newMax);

    glm::vec3 getDirection();
    glm::vec3 getUp();

    float getSensitivity();
    float getFOV();
    float getMin();
    float getMax();
};

#endif
