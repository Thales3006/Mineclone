#include "world/terrain/block/block.h"

Block::Block(BlockID id, bool solid)
    : id(id), solid(id == BlockID::red_flower || id == BlockID::yellow_flower
                        ? false
                        : solid), // temporary translucent fix
      faces(id != BlockID::air ? ALL_FACE : NO_FACE) {}

Block::Block() : id(BlockID::air), solid(false), faces(NO_FACE) {}
