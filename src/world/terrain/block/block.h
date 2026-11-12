#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

enum class BlockID {
    air,
    stone,
    dirt,
    grass_block,
    bricks,
    tnt,
    smooth_stone,
    oak_planks,
    red_flower,
    yellow_flower,
    grass,
};

enum class BlockTextureID {
    stone,
    dirt,
    grass_block_side,
    grass_block_top,
    bricks,
    tnt_side,
    tnt_top,
    tnt_bottom,
    smooth_stone,
    oak_planks,
    red_flower,
    yellow_flower,
    grass,
};

class Block {
  public:
    BlockID id;
    bool solid;

    Block();
    Block(BlockID id, bool solid);
};

#endif
