#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glm/glm.hpp>

#define FRONT_FACE 0b00000001
#define BACK_FACE  0b00000010
#define LEFT_FACE  0b00000100
#define RIGHT_FACE 0b00001000
#define UP_FACE    0b00010000
#define DOWN_FACE  0b00100000

#define ALL_FACE RIGHT_FACE + LEFT_FACE + FRONT_FACE + BACK_FACE + UP_FACE + DOWN_FACE
#define NO_FACE    0b00000000

class Block {
    public:
        unsigned int ID;
        bool solid;

        unsigned char faces;
        
        Block();
        Block(unsigned int ID, bool isSolid);
};

#endif
