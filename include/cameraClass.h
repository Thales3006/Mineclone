#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI_2
#define M_PI_2 1.57079
#endif
class Camera{
    private:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;

        //angles
        float yaw = 0;
        float pitch = 0;

        //keys
        int forward_key = GLFW_KEY_UP;
        int back_key = GLFW_KEY_DOWN;
        int left_key = GLFW_KEY_LEFT;
        int right_key = GLFW_KEY_RIGHT;
        int up_key = GLFW_KEY_RIGHT_SHIFT;
        int down_key = GLFW_KEY_RIGHT_CONTROL;

        float sensitivity = 0.005f;
        float fov = M_PI_2/2;

    public:
        Camera(glm::vec3 pos, float newYaw, float newPitch){
            position = pos;
            setCameraDirection(newYaw, newPitch);
            up = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::mat4 viewMat(){
            return glm::lookAt(position, position+direction, up);
        }

        void setKeys(int forward, int back, int left, int right, int upward, int downward){
            forward_key = forward;
            back_key = back;
            left_key = left;
            right_key = right;
            up_key = upward;
            down_key = downward;
        }

        void cameraMovement(GLFWwindow* window){
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

        void processMouseMovement(double xoffset, double yoffset){
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw   += xoffset;
            pitch += yoffset;

            if (pitch > M_PI_2)
                pitch = M_PI_2;
            if (pitch < -M_PI_2)
                pitch = -M_PI_2;

            setCameraDirection(yaw, pitch);
        }

        void setCameraDirection(float newYaw, float newPitch){
            yaw = newYaw;
            pitch = newPitch;
            direction = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
        }
};
#endif