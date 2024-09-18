#include "entityClass.h"

Entity::Entity(glm::vec3 pos, glm::vec3 siz){
    position = pos;
    size = siz;
}

bool colision(glm::vec3 entity1Pos, glm::vec3 entity1size, glm::vec3 entity2Pos, glm::vec3 entity2size){
    return 
        entity1Pos.x <= entity2Pos.x + entity2size.x && entity1Pos.x + entity1size.x >= entity2Pos.x && 
        entity1Pos.y <= entity2Pos.y + entity2size.y && entity1Pos.y + entity1size.y >= entity2Pos.y &&
        entity1Pos.z <= entity2Pos.z + entity2size.z && entity1Pos.z + entity1size.z >= entity2Pos.z; 
}

float Entity::colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize){
    if(velocity == glm::vec3(0.0)) return 1;

    glm::vec3 near = ((staticPos - size) - position) / velocity;
    glm::vec3 far = ((staticPos - size) + (staticSize + size) - position) / velocity;

    if(near.x > far.x) std::swap(near.x, far.x);
    if(near.y > far.y) std::swap(near.y, far.y);
    if(near.z > far.z) std::swap(near.z, far.z);

    if(
        near.x > far.y || near.x > far.z ||
        near.y > far.x || near.y > far.z ||
        near.z > far.x || near.z > far.y
    ) return 1;
    
    
    float nearTime = near.x > near.y? (std::isnan(near.x)? 0 : near.x) :(std::isnan(near.y)? 0 : near.y);
    nearTime = nearTime > near.z? nearTime : (std::isnan(near.z)? 0 : near.z);

    if(nearTime < 0 || nearTime > 1)
        return 1;

    return nearTime;
}

#include <stdio.h>
void Entity::update(std::vector<Block> blocks){
    float friction = 0.4;
    float time;

    for(Block block : blocks){
        time = colisionContinuous(block.position, glm::vec3(1.0));
        if(time != 1)
            break;
    }
    position += velocity * time;
    if(time != 1) {
        velocity = glm::vec3(0.0);
        printf("colision X: %f Y:%f Z:%f\n",position.x, position.y, position.z);
        return;
    }

    velocity -= velocity * glm::vec3(friction);
}