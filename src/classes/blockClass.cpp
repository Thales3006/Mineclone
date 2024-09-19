#include "blockClass.h"

Block::Block(glm::vec3 pos, unsigned int newID, bool isSolid){
    position = pos;
    ID = newID;
    solid = isSolid;
}