#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glm/glm.hpp>

class Block {
    public:
        unsigned int ID;
        bool solid;
        
        Block();
        Block(unsigned int ID, bool isSolid);
};

#endif