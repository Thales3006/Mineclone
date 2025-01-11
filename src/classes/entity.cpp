#include "entity.h"

Entity::Entity(){
    this->velocity = glm::vec3(0.0);
    this->acceleration = glm::vec3(32.0, 7.0, 32.0);

    this->position = glm::vec3(0.0);
    this->size = glm::vec3(0.0);

    this->chunkx = 0;
    this->chunkz = 0;
    
    this->gravity = 20;
    this->onGround = false;
}

Entity::Entity(glm::vec3 pos, glm::vec3 size){
    this->velocity = glm::vec3(0.0);
    this->acceleration = glm::vec3(32.0, 7.0, 32.0);

    this->position = pos;
    this->size = size;
    
    this->gravity = 20;
    this->onGround = false;
}

void Entity::update(ChunkManager& chunkManager, float deltaTime){

    glm::vec3 friction = glm::vec3(8.0, 0.1, 8.0);
    glm::vec3 correction = glm::vec3(0.0);
    glm::vec3 newCorrection = glm::vec3(0.0);

    if(velocity != glm::vec3(0.0f)){
        glm::vec3 dV = velocity * glm::vec3(deltaTime);
        glm::vec3 nV = glm::vec3(velocity.x >= 0? 1 : -1, velocity.y >= 0? 1 : -1, velocity.z >= 0? 1 : -1);
        
        for (        int x = 0; (dV.x == 0)? (x < 1) : ((dV.x > 0)? (x <= dV.x) : (x >= dV.x)); x += nV.x){
            for (    int y = 0; (dV.y == 0)? (x < 1) : ((dV.y > 0)? (x <= dV.y) : (x >= dV.y)); y += nV.y){
                for (int z = 0; (dV.z == 0)? (x < 1) : ((dV.z > 0)? (x <= dV.z) : (x >= dV.z)); z += nV.z){
                    const glm::vec3 colisionPos = glm::vec3(x,y,z) + position;
                    const glm::vec3 chunkOffSet = Chunk::chunkOffSet(colisionPos);
                    const Block& block = chunkManager.getBlock(chunkx, chunkz, colisionPos); 

                    if(block.solid == false)
                        continue;
                    
                    newCorrection = colisionContinuous(, glm::vec3(1.0f), deltaTime);
                    correction.x = std::abs(correction.x) > std::abs(newCorrection.x)? correction.x : newCorrection.x;
                    correction.y = std::abs(correction.y) > std::abs(newCorrection.y)? correction.y : newCorrection.y;
                    correction.z = std::abs(correction.z) > std::abs(newCorrection.z)? correction.z : newCorrection.z;
                }
            }
        }
    }

        position += velocity * deltaTime + correction * glm::vec3(1.01); // glm::vec3(1.001) only here because of bad colision

    while(position.x < 0){
        position =  glm::vec3(position.x + CHUNK_WIDTH, position.y, position.z);
        chunkx--;
    }
    while(position.x >= CHUNK_WIDTH){
        position =  glm::vec3(position.x - CHUNK_WIDTH, position.y, position.z);
        chunkx++;
    }
    while(position.z < 0){
        position =  glm::vec3(position.x, position.y, position.z + CHUNK_WIDTH);
        chunkz--;
    }
    while(position.z >= CHUNK_WIDTH){
        position =  glm::vec3(position.x, position.y, position.z - CHUNK_WIDTH);
        chunkz++;
    }

    if(correction.x != 0)
        velocity.x = 0;
    if(correction.z != 0)
        velocity.z = 0;
    if(correction.z != 0)
        velocity.y = 0;
    else 
        velocity.y -= gravity * deltaTime;

    velocity -= velocity * friction * glm::vec3(deltaTime);

    onGround = correction.y > 0? true : false;
}

glm::vec3 Entity::colisionContinuous(glm::vec3 staticPos, glm::vec3 staticSize, float deltaTime){
    if(velocity == glm::vec3(0.0)) return glm::vec3(0.0);

    glm::vec3 deltaVel = velocity * deltaTime;

    glm::vec3 near = ((staticPos - size) - Entity::position) / deltaVel;
    glm::vec3 far = ((staticPos - size) + (staticSize + size) - Entity::position) / deltaVel;

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

    return (deltaVel * normal) * (1 - nearTime);
}

bool Entity::colision(glm::vec3 entity1Pos, glm::vec3 entity1size, glm::vec3 entity2Pos, glm::vec3 entity2size){
    return 
        entity1Pos.x <= entity2Pos.x + entity2size.x && entity1Pos.x + entity1size.x >= entity2Pos.x && 
        entity1Pos.y <= entity2Pos.y + entity2size.y && entity1Pos.y + entity1size.y >= entity2Pos.y &&
        entity1Pos.z <= entity2Pos.z + entity2size.z && entity1Pos.z + entity1size.z >= entity2Pos.z; 
}