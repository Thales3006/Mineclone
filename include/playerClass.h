#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "cameraClass.h"
#include "entityClass.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "entityClass.h"
#include "blockClass.h"
#include <vector>

#define MAX_VELOCITY 3.0

class Player: public Camera {
    public:
        glm::vec3 velocity = glm::vec3(0.0);
        glm::vec3 acceleration = glm::vec3(0.04, 0.025, 0.04);
        float gravity = 0.001;

        int forward_key = GLFW_KEY_UP;
        int back_key = GLFW_KEY_DOWN;
        int left_key = GLFW_KEY_LEFT;
        int right_key = GLFW_KEY_RIGHT;
        int up_key = GLFW_KEY_RIGHT_SHIFT;
        int down_key = GLFW_KEY_RIGHT_CONTROL;

        Player(glm::vec3 pos);

        void setKeys(int forward, int back, int left, int right, int upward, int downward);

        void processKeyMovement(GLFWwindow* window);
        void processMouseMovement(double xoffset, double yoffset);

        void update(std::vector<Block> blocks);
};

#endif