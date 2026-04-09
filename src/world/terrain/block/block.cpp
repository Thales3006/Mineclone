#include "world/terrain/block/block.h"

Block::Block(BlockID id, bool solid)
    : id(id), solid(id == BlockID::red_flower || id == BlockID::yellow_flower
                        ? false
                        : solid) // temporary translucent fix
{}

Block::Block() : id(BlockID::air), solid(false) {}
