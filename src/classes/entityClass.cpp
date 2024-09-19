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

bool Entity::colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize, glm::vec3 &correction){
    if(velocity == glm::vec3(0.0)) return false;

    glm::vec3 near = ((staticPos - size) - Entity::position) / velocity;
    glm::vec3 far = ((staticPos - size) + (staticSize + size) - Entity::position) / velocity;

    if(near.x > far.x) std::swap(near.x, far.x);
    if(near.y > far.y) std::swap(near.y, far.y);
    if(near.z > far.z) std::swap(near.z, far.z);

    if(
        near.x > far.y || near.x > far.z ||
        near.y > far.x || near.y > far.z ||
        near.z > far.x || near.z > far.y
    ) return false;
    
    
    float nearTime = near.x > near.y? (std::isnan(near.x)? -1 : near.x) :(std::isnan(near.y)? -1 : near.y);
    nearTime = nearTime > near.z? nearTime : (std::isnan(near.z)? -1 : near.z);

    if(nearTime < 0 || nearTime > 1)
        return false;

    glm::vec3 normal;
    near.x > near.y?
        near.x > near.z?
            normal = glm::vec3(-1, 0, 0) :
            normal = glm::vec3(0, 0, -1) :
        near.y > near.z?
            normal = glm::vec3(0, -1, 0) :
            normal = glm::vec3(0, 0, -1) ;

    glm::vec3 newCorrection = (velocity * normal) * (1 - nearTime);

    correction.x = std::abs(correction.x) > std::abs(newCorrection.x)? correction.x : newCorrection.x;
    correction.y = std::abs(correction.y) > std::abs(newCorrection.y)? correction.y : newCorrection.y;
    correction.z = std::abs(correction.z) > std::abs(newCorrection.z)? correction.z : newCorrection.z;

    return true;
}

#include <stdio.h>
bool Entity::update(std::vector<Block> blocks){
    glm::vec3 friction = glm::vec3(0.4, 0.1, 0.4);
    glm::vec3 correction = glm::vec3(0.0);

    for(Block block : blocks)
        colisionContinuous(block.position, glm::vec3(1.0), correction);

    position += velocity + correction;

    velocity.y -= gravity;
    velocity -= velocity * glm::vec3(friction);

    return correction.y > 0? true : false;
}