#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "block.h"

#include "glm/glm.hpp"
#include <vector>

class Entity {
    public:
        glm::vec3 size;
        glm::vec3 position;

        glm::vec3 velocity;
        glm::vec3 acceleration;

        float gravity;
        bool onGround;
    
        Entity();
        Entity(glm::vec3 pos, glm::vec3 size);
        void update(std::vector<Block> blocks);
        glm::vec3 colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize);
        
};

#endif