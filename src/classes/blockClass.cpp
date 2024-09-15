#include "blockClass.h"

Block::Block(glm::vec3 pos, bool isSolid){
    position = pos;
    solid = isSolid;
}