#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#define FRONT_FACE 0b00000001
#define BACK_FACE 0b00000010
#define LEFT_FACE 0b00000100
#define RIGHT_FACE 0b00001000
#define UP_FACE 0b00010000
#define DOWN_FACE 0b00100000

#define ALL_FACE                                                               \
    RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE
#define NO_FACE 0b00000000

enum class BlockID {
    air,
    stone,
    dirt,
    grass,
    brick_block,
    tnt,
    smooth_stone,
    planks,
    red_flower,
    yellow_flower,
};

class Block {
  public:
    BlockID id;
    bool solid;

    unsigned char faces;

    Block();
    Block(BlockID id, bool solid);
};

#endif
