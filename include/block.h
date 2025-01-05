#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glm/glm.hpp>

class Block {
    public:
        glm::vec3 position;
        unsigned int ID;
        bool solid;
        Block(glm::vec3 pos, unsigned int ID, bool isSolid);
};

#endif