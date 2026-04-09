#include "world/terrain/chunk/chunkRegion.h"

Block ChunkRegion::getBlock(int x, int y, int z) {
    int ox = (x < 0) ? -1 : (x >= CHUNK_WIDTH) ? 1 : 0;
    int oz = (z < 0) ? -1 : (z >= CHUNK_WIDTH) ? 1 : 0;

    int localX = x - ox * CHUNK_WIDTH;
    int localZ = z - oz * CHUNK_WIDTH;

    if (y < 0 || y >= CHUNK_HEIGHT)
        return Block();

    const std::optional<std::shared_ptr<Chunk>> *neighbors[3][3] = {
        {&backLeft, &left, &frontLeft},
        {&back, nullptr, &front},
        {&backRight, &right, &frontRight},
    };

    if (ox == 0 && oz == 0)
        return main->blocks[localX][y][localZ];

    auto &neighbor = neighbors[ox + 1][oz + 1];
    if (neighbor && neighbor->has_value())
        return neighbor->value()->blocks[localX][y][localZ];

    return Block();
}

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

std::array<float, 8> ChunkRegion::getBlockOcclusion(int x, int y, int z) {
    std::array<float, 8> occlusion;
    occlusion.fill(1.0f);

    auto ao = [this](int x, int y, int z) -> float {
        return getBlock(x, y, z).solid ? 0.6f : 1.0f;
    };

    occlusion[Node::frontRightUp] =
        ao(x + 1, y + 1, z + 1) * ao(x, y + 1, z + 1) * ao(x + 1, y + 1, z);
    occlusion[Node::frontLeftUp] =
        ao(x - 1, y + 1, z + 1) * ao(x, y + 1, z + 1) * ao(x - 1, y + 1, z);
    occlusion[Node::backRightUp] =
        ao(x + 1, y + 1, z - 1) * ao(x, y + 1, z - 1) * ao(x + 1, y + 1, z);
    occlusion[Node::backLeftUp] =
        ao(x - 1, y + 1, z - 1) * ao(x, y + 1, z - 1) * ao(x - 1, y + 1, z);

    occlusion[Node::frontRightDown] = ao(x + 1, y - 1, z + 1);
    occlusion[Node::frontLeftDown] =
        ao(x - 1, y - 1, z + 1) * ao(x, y - 1, z + 1) * ao(x - 1, y - 1, z);
    occlusion[Node::backRightDown] =
        ao(x + 1, y - 1, z - 1) * ao(x, y - 1, z - 1) * ao(x + 1, y - 1, z);
    occlusion[Node::backLeftDown] =
        ao(x - 1, y - 1, z - 1) * ao(x, y - 1, z - 1) * ao(x - 1, y - 1, z);

    return occlusion;
}
