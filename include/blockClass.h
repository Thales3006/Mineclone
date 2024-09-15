#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glm/glm.hpp>

class Block {
    public:
        glm::vec3 position;
        bool solid;
        Block(glm::vec3 pos, bool isSolid);
};

#endif