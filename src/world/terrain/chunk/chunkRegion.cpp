#include "world/terrain/chunk/chunkRegion.h"

std::optional<RegionSnapshot> ChunkRegion::lock() const {
    std::shared_ptr<Chunk> m = main.lock();
    if (!m)
        return std::nullopt;

    return RegionSnapshot{
        .main = std::move(m),
        .right = right.lock(),
        .left = left.lock(),
        .front = front.lock(),
        .back = back.lock(),
        .frontRight = frontRight.lock(),
        .backRight = backRight.lock(),
        .frontLeft = frontLeft.lock(),
        .backLeft = backLeft.lock(),
    };
}

Block RegionSnapshot::getBlock(int x, int y, int z) const {
    int ox = (x < 0) ? -1 : (x >= CHUNK_WIDTH) ? 1 : 0;
    int oz = (z < 0) ? -1 : (z >= CHUNK_WIDTH) ? 1 : 0;

    int localX = x - ox * CHUNK_WIDTH;
    int localZ = z - oz * CHUNK_WIDTH;

    if (y < 0 || y >= CHUNK_HEIGHT)
        return Block();

    const std::shared_ptr<Chunk> *neighbors[3][3] = {
        {&backLeft, &left, &frontLeft},
        {&back, nullptr, &front},
        {&backRight, &right, &frontRight},
    };

    if (ox == 0 && oz == 0)
        return main->blocks[localX][y][localZ];

    auto &neighbor = neighbors[ox + 1][oz + 1];
    if (neighbor && *neighbor)
        return (*neighbor)->blocks[localX][y][localZ];

    return Block();
}

unsigned char RegionSnapshot::getBlockFaces(int x, int y, int z) const {
    if (main->blocks[x][y][z].id == BlockID::air) {
        return NO_FACE;
    }

    unsigned char faces = ALL_FACE;
    if (x != CHUNK_WIDTH - 1 ? main->blocks[x + 1][y][z].solid != false
        : right              ? right->blocks[0][y][z].solid != false
                             : false) {
        faces &= ~RIGHT_FACE;
    }
    if (x != 0 ? main->blocks[x - 1][y][z].solid != false
        : left ? left->blocks[CHUNK_WIDTH - 1][y][z].solid != false
               : false) {
        faces &= ~LEFT_FACE;
    }
    if (z != CHUNK_WIDTH - 1 ? main->blocks[x][y][z + 1].solid != false
        : front              ? front->blocks[x][y][0].solid != false
                             : false) {
        faces &= ~FRONT_FACE;
    }
    if (z != 0 ? main->blocks[x][y][z - 1].solid != false
        : back ? back->blocks[x][y][CHUNK_WIDTH - 1].solid != false
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

std::array<float, 4> RegionSnapshot::getBlockOcclusion(int x, int y, int z,
                                                     unsigned char face) const {
    std::array<float, 4> occlusion;

    static const glm::ivec3 vertexAO[6][4][3] = {
        {
            // FRONT: v0=(0,0,1) v1=(1,0,1) v2=(1,1,1) v3=(0,1,1)
            {{-1, 0, 1}, {0, -1, 1}, {-1, -1, 1}}, // v0 frontLeftDown
            {{1, 0, 1}, {0, -1, 1}, {1, -1, 1}},   // v1 frontRightDown
            {{1, 0, 1}, {0, 1, 1}, {1, 1, 1}},     // v2 frontRightUp
            {{-1, 0, 1}, {0, 1, 1}, {-1, 1, 1}},   // v3 frontLeftUp
        },
        {
            // BACK: v0=(1,0,0) v1=(0,0,0) v2=(0,1,0) v3=(1,1,0)
            {{1, 0, -1}, {0, -1, -1}, {1, -1, -1}},   // v0 backRightDown
            {{-1, 0, -1}, {0, -1, -1}, {-1, -1, -1}}, // v1 backLeftDown
            {{-1, 0, -1}, {0, 1, -1}, {-1, 1, -1}},   // v2 backLeftUp
            {{1, 0, -1}, {0, 1, -1}, {1, 1, -1}},     // v3 backRightUp
        },
        {
            // LEFT: v0=(0,0,0) v1=(0,0,1) v2=(0,1,1) v3=(0,1,0)
            {{-1, 0, -1}, {-1, -1, 0}, {-1, -1, -1}}, // v0 backLeftDown
            {{-1, 0, 1}, {-1, -1, 0}, {-1, -1, 1}},   // v1 frontLeftDown
            {{-1, 0, 1}, {-1, 1, 0}, {-1, 1, 1}},     // v2 frontLeftUp
            {{-1, 0, -1}, {-1, 1, 0}, {-1, 1, -1}},   // v3 backLeftUp
        },
        {
            // RIGHT: v0=(1,0,1) v1=(1,0,0) v2=(1,1,0) v3=(1,1,1)
            {{1, 0, 1}, {1, -1, 0}, {1, -1, 1}},   // v0 frontRightDown
            {{1, 0, -1}, {1, -1, 0}, {1, -1, -1}}, // v1 backRightDown
            {{1, 0, -1}, {1, 1, 0}, {1, 1, -1}},   // v2 backRightUp
            {{1, 0, 1}, {1, 1, 0}, {1, 1, 1}},     // v3 frontRightUp
        },
        {
            // UP: v0=(0,1,1) v1=(1,1,1) v2=(1,1,0) v3=(0,1,0)
            {{-1, 1, 0}, {0, 1, 1}, {-1, 1, 1}},   // v0 frontLeftUp
            {{1, 1, 0}, {0, 1, 1}, {1, 1, 1}},     // v1 frontRightUp
            {{1, 1, 0}, {0, 1, -1}, {1, 1, -1}},   // v2 backRightUp
            {{-1, 1, 0}, {0, 1, -1}, {-1, 1, -1}}, // v3 backLeftUp
        },
        {
            // DOWN: v0=(0,0,1) v1=(1,0,1) v2=(1,0,0) v3=(0,0,0)
            {{-1, -1, 0}, {0, -1, 1}, {-1, -1, 1}},   // v0 frontLeftDown
            {{1, -1, 0}, {0, -1, 1}, {1, -1, 1}},     // v1 frontRightDown
            {{1, -1, 0}, {0, -1, -1}, {1, -1, -1}},   // v2 backRightDown
            {{-1, -1, 0}, {0, -1, -1}, {-1, -1, -1}}, // v3 backLeftDown
        },
    };

    int fi = __builtin_ctz(face);
    for (int v = 0; v < 4; v++) {
        int s1 = getBlock(x + vertexAO[fi][v][0].x, y + vertexAO[fi][v][0].y,
                          z + vertexAO[fi][v][0].z)
                         .solid
                     ? 1
                     : 0;
        int s2 = getBlock(x + vertexAO[fi][v][1].x, y + vertexAO[fi][v][1].y,
                          z + vertexAO[fi][v][1].z)
                         .solid
                     ? 1
                     : 0;
        int diag =
            (s1 && s2)
                ? 1
                : (getBlock(x + vertexAO[fi][v][2].x, y + vertexAO[fi][v][2].y,
                            z + vertexAO[fi][v][2].z)
                           .solid
                       ? 1
                       : 0);

        static const float aoValues[4] = {0.25f, 0.4f, 0.6f, 1.0f};
        occlusion[v] = aoValues[3 - (s1 + s2 + diag)];
    }
    return occlusion;
}
