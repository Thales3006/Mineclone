#include "block.h"

Block::Block(glm::vec3 pos, unsigned int newID, bool isSolid){
    position = pos;
    ID = newID;
    solid = isSolid;
}

Block::Block(){
    position = glm::vec3(0, 0, 0);
    ID = 0;
    solid = false;
}