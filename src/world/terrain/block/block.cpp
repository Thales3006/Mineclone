#include "world/terrain/block/block.h"

Block::Block(unsigned int ID, bool solid)
    : ID(ID),
      solid(ID == 12 || ID == 14 ? false : solid), // temporary translucent fix
      faces(ID != 0 ? ALL_FACE : NO_FACE) {}

Block::Block() : ID(0), solid(false), faces(NO_FACE) {}
