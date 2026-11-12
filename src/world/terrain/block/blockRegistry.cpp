#include "world/terrain/block/blockRegistry.h"

#include <array>

namespace {

const glm::vec3 kWhite(1.0f);
const glm::vec3 kGreen(0.5f, 0.8f, 0.4f);

const AABB kFullBox{{0, 0, 0}, {1, 1, 1}};
const AABB kFlowerBox{{0.3f, 0, 0.3f}, {0.7f, 0.6f, 0.7f}};
const AABB kGrassBox{{0.2f, 0, 0.2f}, {0.8f, 0.7f, 0.8f}};

BlockDef cube(BlockTextureID texture) {
    BlockDef def;
    def.model = BlockModel::Cube;
    for (int i = 0; i < 6; i++) {
        def.faces[i] = texture;
        def.tint[i] = kWhite;
    }
    def.selection = {kFullBox};
    def.collision = {kFullBox};
    def.occludes = true;
    return def;
}

BlockDef plant(BlockTextureID texture, glm::vec3 tint, AABB box) {
    BlockDef def;
    def.model = BlockModel::Plant;
    for (int i = 0; i < 6; i++) {
        def.faces[i] = texture;
        def.tint[i] = tint;
    }
    def.selection = {box};
    def.collision = {};
    def.occludes = false;
    return def;
}

std::array<BlockDef, 11> buildTable() {
    std::array<BlockDef, 11> table;

    table[(int)BlockID::air] = cube(BlockTextureID::stone);
    table[(int)BlockID::air].selection = {};
    table[(int)BlockID::air].collision = {};
    table[(int)BlockID::air].occludes = false;

    table[(int)BlockID::stone] = cube(BlockTextureID::stone);
    table[(int)BlockID::dirt] = cube(BlockTextureID::dirt);
    table[(int)BlockID::bricks] = cube(BlockTextureID::bricks);
    table[(int)BlockID::smooth_stone] = cube(BlockTextureID::smooth_stone);
    table[(int)BlockID::oak_planks] = cube(BlockTextureID::oak_planks);

    BlockDef grassBlock = cube(BlockTextureID::grass_block_side);
    grassBlock.faces[4] = BlockTextureID::grass_block_top;
    grassBlock.faces[5] = BlockTextureID::dirt;
    grassBlock.tint[4] = kGreen;
    table[(int)BlockID::grass_block] = grassBlock;

    BlockDef tnt = cube(BlockTextureID::tnt_side);
    tnt.faces[4] = BlockTextureID::tnt_top;
    tnt.faces[5] = BlockTextureID::tnt_bottom;
    table[(int)BlockID::tnt] = tnt;

    table[(int)BlockID::red_flower] =
        plant(BlockTextureID::red_flower, kWhite, kFlowerBox);
    table[(int)BlockID::yellow_flower] =
        plant(BlockTextureID::yellow_flower, kWhite, kFlowerBox);
    table[(int)BlockID::grass] = plant(BlockTextureID::grass, kGreen, kGrassBox);

    return table;
}

} // namespace

const BlockDef &blockDef(BlockID id) {
    static const std::array<BlockDef, 11> table = buildTable();
    return table[static_cast<int>(id)];
}
