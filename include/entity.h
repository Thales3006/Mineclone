#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "chunk.h"

#include "glm/glm.hpp"
#include <map>
#include <tuple>

class Entity {
    public:
        glm::vec3 size;
        glm::vec3 position;

        int chunkx;
        int chunkz;

        glm::vec3 velocity;
        glm::vec3 acceleration;

        float gravity;
        bool onGround;
    
        Entity();
        Entity(glm::vec3 pos, glm::vec3 size);

        void update(const std::map<std::tuple<int,int>,Chunk>& chunks, float deltaTime);
        glm::vec3 colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize, float deltaTime);
        static bool colision(glm::vec3 entity1Pos, glm::vec3 entity1size, glm::vec3 entity2Pos, glm::vec3 entity2size);
        
};

#endif