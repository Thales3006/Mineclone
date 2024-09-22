#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "blockClass.h"

#define chunkheight 256
#define chunkwidth 16
#define chunkdepth 16 

class Chunk {
    public:
        int position[3];
        Block block[chunkwidth][chunkheight][chunkdepth];

        Chunk(int x, int y, int z) {
            position[0] = x;
            position[1] = y;
            position[2] = z;

            for(int i=0; i<chunkwidth; i++){
                for(int j=0; j<chunkheight; j++){
                    for(int k=0; k<chunkdepth; k++){
                        block[i][j][k].position = glm::vec3(i, j, k);
                        block[i][j][k].ID = 0;
                        block[i][j][k].solid = false;
                    }
                }
            }
        }
};

#endif