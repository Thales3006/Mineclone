#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <cmath>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Camera{
    private:
        float sensitivity = 0.005f;
        float FOV = M_PI/4;

    public:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;

        //angles
        float yaw = 0;
        float pitch = 0;

        Camera(glm::vec3 pos, float newYaw, float newPitch);

        glm::mat4 getMatrixView();
        glm::mat4 getMatrixProjection(float aspect);

        void setDirection(float newYaw, float newPitch);
        void setSensitivity(float newSensitivity);
        void setFOV(float newFOV);

        glm::vec3 getDirection();
        float getSensitivity();
        float getFOV();
};

#endif