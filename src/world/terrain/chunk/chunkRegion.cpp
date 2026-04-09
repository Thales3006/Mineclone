#include "world/terrain/chunk/chunkRegion.h"

unsigned char ChunkRegion::getBlockFaces(int x, int y, int z) {
    if (main->blocks[x][y][z].id == BlockID::air) {
        return NO_FACE;
    }

    unsigned char faces = ALL_FACE;
    if (x != CHUNK_WIDTH - 1 ? main->blocks[x + 1][y][z].solid != false
        : right.has_value()  ? right.value()->blocks[0][y][z].solid != false
                             : false) {
        faces &= ~RIGHT_FACE;
    }
    if (x != 0 ? main->blocks[x - 1][y][z].solid != false
        : left.has_value()
            ? left.value()->blocks[CHUNK_WIDTH - 1][y][z].solid != false
            : false) {
        faces &= ~LEFT_FACE;
    }
    if (z != CHUNK_WIDTH - 1 ? main->blocks[x][y][z + 1].solid != false
        : front.has_value()  ? front.value()->blocks[x][y][0].solid != false
                             : false) {
        faces &= ~FRONT_FACE;
    }
    if (z != 0 ? main->blocks[x][y][z - 1].solid != false
        : back.has_value()
            ? back.value()->blocks[x][y][CHUNK_WIDTH - 1].solid != false
            : false) {
        faces &= ~BACK_FACE;
    }
    if (y != CHUNK_HEIGHT - 1 && main->blocks[x][y + 1][z].solid != false) {
        faces &= ~UP_FACE;
    }
    if (y != 0 && main->blocks[x][y - 1][z].solid != false) {
        faces &= ~DOWN_FACE;
    }
    return faces;
}
