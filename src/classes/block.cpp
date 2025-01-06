#include "block.h"

Block::Block(unsigned int newID, bool isSolid){
    ID = newID;
    solid = isSolid;
}

Block::Block(){
    ID = 0;
    solid = false;
}