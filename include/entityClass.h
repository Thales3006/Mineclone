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
        glm::vec3 acceleration = glm::vec3(0.04, 0.4, 0.04);
        float gravity = 0.02;
    
        Entity(glm::vec3 pos, glm::vec3 siz);
        bool update(std::vector<Block> blocks);
        bool colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize, glm::vec3 &correction);
        
};

#endif