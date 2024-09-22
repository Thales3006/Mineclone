#define chunkheight 256
#define chunkwidth 16
#define chunkdepth 16 
#include <stdio.h>
#include <array>

#include "include/glm/glm.hpp"
class Block {
    public:
        glm::vec3 position;
        unsigned int ID;
        bool solid;
        Block(glm::vec3 pos, unsigned int ID, bool isSolid);
};

Block::Block(glm::vec3 pos, unsigned int newID, bool isSolid){
    position = pos;
    ID = newID;
    solid = isSolid;
}


int main(){
    printf("Hello World\n");
    std::array<Block,10> a;
}