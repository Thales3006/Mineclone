#include "world/terrain/block/block.h"

Block::Block(unsigned int newID, bool isSolid) {
    ID = newID;
    solid = isSolid;

    if (newID)
        faces = RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE;
    else
        faces = 0;
}

Block::Block() {
    ID = 0;
    solid = false;
    faces = 0b00111111;
}
