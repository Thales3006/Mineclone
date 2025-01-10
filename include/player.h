#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "camera.h"
#include "entity.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class Player: public Entity {
    public:
        Camera camera;

        int forward_key;
        int back_key;
        int left_key;
        int right_key;
        int up_key;
        int down_key;

        Player();
        Player(glm::vec3 pos, glm::vec3 siz);

        void setKeys(int forward, int back, int left, int right, int upward, int downward);

        void updatePlayer(GLFWwindow* window, const std::map<std::tuple<int,int>,Chunk>& chunks, float deltaTime);

        void processKeyMovement(GLFWwindow* window, float deltaTime);
        void processMouseMovement(double xoffset, double yoffset);
};

#endif