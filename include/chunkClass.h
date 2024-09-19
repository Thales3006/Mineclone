#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "blockClass.h"

#define chunkheight 256
#define chunkwidth 16
#define chunkdepth 16 

class Chunk {
    public:
        int position[3];
        int block[chunkwidth][chunkheight][chunkdepth] = {0};

        Chunk(int x, int y, int z) {
            position[0] = x;
            position[1] = y;
            position[2] = z;

        }
}

#endif