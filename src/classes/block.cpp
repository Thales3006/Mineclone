#include "block.h"

Block::Block(unsigned int newID, bool isSolid){
    ID = newID;
    solid = isSolid;

    faces = RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE;
}

Block::Block(){
    ID = 0;
    solid = false;
    faces = 0b00111111;
}