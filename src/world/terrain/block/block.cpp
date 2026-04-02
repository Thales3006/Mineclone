#include "world/terrain/block/block.h"

Block::Block(unsigned int ID, bool solid) : ID(ID), solid(solid) {
    if (ID != 0)
        faces = RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE;
    else
        faces = NO_FACE;
}

Block::Block() : ID(0), solid(false), faces(NO_FACE) {}
