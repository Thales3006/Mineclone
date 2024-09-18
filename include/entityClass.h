#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "blockClass.h"

#include "glm/glm.hpp"
#include <vector>

class Entity {
    public:
        glm::vec3 size;
        glm::vec3 position;

        glm::vec3 velocity = glm::vec3(0.0);
        glm::vec3 acceleration = glm::vec3(0.04, 0.025, 0.04);
        float gravity = 0.001;
    
        Entity(glm::vec3 pos, glm::vec3 siz);
        void update(std::vector<Block> blocks);
        float colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize);
        
};

#endif