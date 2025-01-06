#include "entity.h"

Entity::Entity(){
    this->velocity = glm::vec3(0.0);
    this->acceleration = glm::vec3(0.0);

    this->position = glm::vec3(0.0);
    this->size = glm::vec3(0.0);
    
    this->gravity = 0.02;
    this->onGround = false;
}

Entity::Entity(glm::vec3 pos, glm::vec3 size){
    this->velocity = glm::vec3(0.0);
    this->acceleration = glm::vec3(0.04, 0.04, 0.04);

    this->position = pos;
    this->size = size;
    
    this->gravity = 0.02;
    this->onGround = false;
}

void Entity::update(const std::vector<Chunk>& chunks){
    glm::vec3 friction = glm::vec3(0.4, 0.1, 0.4);
    glm::vec3 correction = glm::vec3(0.0);
    glm::vec3 newCorrection = glm::vec3(0.0);

    for(const auto& chunk : chunks)
        for (int x = 0; x < CHUNK_WIDTH; x++)
            for (int y = 0; y < CHUNK_HEIGHT; y++)
                for (int z = 0; z < CHUNK_WIDTH; z++) {
                    if(!chunk.blocks[x][y][z].ID)
                        continue;
                    newCorrection = colisionContinuous(glm::vec3(x,y,z) + glm::vec3(chunk.position[0],0,chunk.position[1])*glm::vec3(CHUNK_WIDTH), glm::vec3(1.0));
                    correction.x = std::abs(correction.x) > std::abs(newCorrection.x)? correction.x : newCorrection.x;
                    correction.y = std::abs(correction.y) > std::abs(newCorrection.y)? correction.y : newCorrection.y;
                    correction.z = std::abs(correction.z) > std::abs(newCorrection.z)? correction.z : newCorrection.z;
                }

    position += velocity + correction * glm::vec3(1.001); // glm::vec3(1.001) only here because of bad colision

    //velocity.y -= gravity;
    velocity -= velocity * glm::vec3(friction);

    onGround = correction.y > 0? true : false;
}

glm::vec3 Entity::colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize){
    if(velocity == glm::vec3(0.0)) return glm::vec3(0.0);

    glm::vec3 near = ((staticPos - size) - Entity::position) / velocity;
    glm::vec3 far = ((staticPos - size) + (staticSize + size) - Entity::position) / velocity;

    if(near.x > far.x) std::swap(near.x, far.x);
    if(near.y > far.y) std::swap(near.y, far.y);
    if(near.z > far.z) std::swap(near.z, far.z);

    if(
        near.x > far.y || near.x > far.z ||
        near.y > far.x || near.y > far.z ||
        near.z > far.x || near.z > far.y
    ) return glm::vec3(0.0);
    
    
    float nearTime = near.x > near.y? (std::isnan(near.x)? -1 : near.x) :(std::isnan(near.y)? -1 : near.y);
    nearTime = nearTime > near.z? nearTime : (std::isnan(near.z)? -1 : near.z);

    if(nearTime < 0 || nearTime > 1)
        return glm::vec3(0.0);

    glm::vec3 normal;
    near.x > near.y?
        near.x > near.z?
            normal = glm::vec3(-1, 0, 0) :
            normal = glm::vec3(0, 0, -1) :
        near.y > near.z?
            normal = glm::vec3(0, -1, 0) :
            normal = glm::vec3(0, 0, -1) ;

    return (velocity * normal) * (1 - nearTime);
}

bool colision(glm::vec3 entity1Pos, glm::vec3 entity1size, glm::vec3 entity2Pos, glm::vec3 entity2size){
    return 
        entity1Pos.x <= entity2Pos.x + entity2size.x && entity1Pos.x + entity1size.x >= entity2Pos.x && 
        entity1Pos.y <= entity2Pos.y + entity2size.y && entity1Pos.y + entity1size.y >= entity2Pos.y &&
        entity1Pos.z <= entity2Pos.z + entity2size.z && entity1Pos.z + entity1size.z >= entity2Pos.z; 
}