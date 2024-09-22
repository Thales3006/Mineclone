#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "cameraClass.h"
#include "entityClass.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class Player: public Camera, public Entity {
    public:

        int forward_key = GLFW_KEY_UP;
        int back_key = GLFW_KEY_DOWN;
        int left_key = GLFW_KEY_LEFT;
        int right_key = GLFW_KEY_RIGHT;
        int up_key = GLFW_KEY_RIGHT_SHIFT;
        int down_key = GLFW_KEY_RIGHT_CONTROL;

        bool onGround = true;

        Player(glm::vec3 pos, glm::vec3 siz);

        void setKeys(int forward, int back, int left, int right, int upward, int downward);

        void updatePlayer(GLFWwindow* window, std::vector<Chunk> chunks);

        void processKeyMovement(GLFWwindow* window);
        void processMouseMovement(double xoffset, double yoffset);
};

#endif