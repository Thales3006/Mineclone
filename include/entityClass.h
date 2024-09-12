#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "glm/glm.hpp"

class Entity {
    public:
        float height=1.8;
        float width=0.9;
        glm::vec3 position;
    
        Entity(glm::vec3 pos, float h, float w);
};

#endif