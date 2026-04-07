#include "render/mesh/terrainVertexMap.h"

TerrainVertexMap::TerrainVertexMap(
    std::shared_ptr<TextureManager> textureManager)
    : textureManager(textureManager) {
    fullBlock[FRONT_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)},
            {glm::vec3(1, 0, 1), glm::vec3(0, 0, 1)},
            {glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)},
            {glm::vec3(0, 1, 1), glm::vec3(0, 0, 1)},
        },
        {0, 1, 3, 1, 2, 3},
    };

    fullBlock[BACK_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)},
            {glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)},
            {glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)},
            {glm::vec3(1, 1, 0), glm::vec3(0, 0, -1)},
        },
        {2, 3, 0, 2, 0, 1},
    };

    fullBlock[RIGHT_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(1, 0, 1), glm::vec3(1, 0, 0)},
            {glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)},
            {glm::vec3(1, 1, 0), glm::vec3(1, 0, 0)},
            {glm::vec3(1, 1, 1), glm::vec3(1, 0, 0)},
        },
        {0, 1, 3, 1, 2, 3},
    };

    fullBlock[LEFT_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec2(1, 0)},
            {glm::vec3(0, 0, 1), glm::vec3(-1, 0, 0), glm::vec2(0, 0)},
            {glm::vec3(0, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(0, 1)},
            {glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), glm::vec2(1, 1)},
        },
        {2, 3, 0, 2, 0, 1},
    };

    fullBlock[UP_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(0, 1, 1), glm::vec3(0, 1, 0)},
            {glm::vec3(1, 1, 1), glm::vec3(0, 1, 0)},
            {glm::vec3(1, 1, 0), glm::vec3(0, 1, 0)},
            {glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)},
        },
        {0, 1, 3, 1, 2, 3},
    };

    fullBlock[DOWN_FACE] = Geometry<TerrainVertex>{
        {
            {glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)},
            {glm::vec3(1, 0, 1), glm::vec3(0, -1, 0)},
            {glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)},
            {glm::vec3(0, 0, 0), glm::vec3(0, -1, 0)},
        },
        {0, 3, 2, 0, 2, 1},
    };
}

Geometry<TerrainVertex> TerrainVertexMap::getUVFace(BlockTextureID id,
                                                    unsigned char face) {
    Geometry<TerrainVertex> faceGeometry = fullBlock[face];
    std::array<glm::vec2, 4> tex = textureManager->getBlockUV(id);
    for (int i = 0; i < 4; i++) {
        faceGeometry.vertices[i].uv = tex[i];
    }
    return faceGeometry;
}

#define ADD_FACE(TEXTURE_ID, FACE)                                             \
    if (block.faces & FACE)                                                    \
        blockGeometry.append(getUVFace(TEXTURE_ID, FACE));

Geometry<TerrainVertex> TerrainVertexMap::getGeometry(Block &block) {
    Geometry<TerrainVertex> blockGeometry{};
    switch (block.id) {
    case BlockID::air:
        return Geometry<TerrainVertex>{};
    case BlockID::stone:
        ADD_FACE(BlockTextureID::stone, UP_FACE)
        ADD_FACE(BlockTextureID::stone, DOWN_FACE)
        ADD_FACE(BlockTextureID::stone, FRONT_FACE)
        ADD_FACE(BlockTextureID::stone, BACK_FACE)
        ADD_FACE(BlockTextureID::stone, LEFT_FACE)
        ADD_FACE(BlockTextureID::stone, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::dirt:
        ADD_FACE(BlockTextureID::dirt, UP_FACE)
        ADD_FACE(BlockTextureID::dirt, DOWN_FACE)
        ADD_FACE(BlockTextureID::dirt, FRONT_FACE)
        ADD_FACE(BlockTextureID::dirt, BACK_FACE)
        ADD_FACE(BlockTextureID::dirt, LEFT_FACE)
        ADD_FACE(BlockTextureID::dirt, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::grass:
        ADD_FACE(BlockTextureID::dirt, DOWN_FACE)
        ADD_FACE(BlockTextureID::grass_side, FRONT_FACE)
        ADD_FACE(BlockTextureID::grass_side, BACK_FACE)
        ADD_FACE(BlockTextureID::grass_side, LEFT_FACE)
        ADD_FACE(BlockTextureID::grass_side, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        if (block.faces & UP_FACE) {
            auto geometry = getUVFace(BlockTextureID::grass_top, UP_FACE);
            for (auto &vertex : geometry.vertices)
                vertex.color = glm::vec3(0.5f, 0.8, 0.4f);
            blockGeometry.append(geometry);
        }
        return blockGeometry;
    case BlockID::brick_block:
        ADD_FACE(BlockTextureID::brick_block, UP_FACE)
        ADD_FACE(BlockTextureID::brick_block, DOWN_FACE)
        ADD_FACE(BlockTextureID::brick_block, FRONT_FACE)
        ADD_FACE(BlockTextureID::brick_block, BACK_FACE)
        ADD_FACE(BlockTextureID::brick_block, LEFT_FACE)
        ADD_FACE(BlockTextureID::brick_block, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::tnt:
        ADD_FACE(BlockTextureID::tnt_top, UP_FACE)
        ADD_FACE(BlockTextureID::tnt_bottom, DOWN_FACE)
        ADD_FACE(BlockTextureID::tnt_side, FRONT_FACE)
        ADD_FACE(BlockTextureID::tnt_side, BACK_FACE)
        ADD_FACE(BlockTextureID::tnt_side, LEFT_FACE)
        ADD_FACE(BlockTextureID::tnt_side, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::smooth_stone:
        ADD_FACE(BlockTextureID::smooth_stone, UP_FACE)
        ADD_FACE(BlockTextureID::smooth_stone, DOWN_FACE)
        ADD_FACE(BlockTextureID::smooth_stone, FRONT_FACE)
        ADD_FACE(BlockTextureID::smooth_stone, BACK_FACE)
        ADD_FACE(BlockTextureID::smooth_stone, LEFT_FACE)
        ADD_FACE(BlockTextureID::smooth_stone, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::planks:
        ADD_FACE(BlockTextureID::planks, UP_FACE)
        ADD_FACE(BlockTextureID::planks, DOWN_FACE)
        ADD_FACE(BlockTextureID::planks, FRONT_FACE)
        ADD_FACE(BlockTextureID::planks, BACK_FACE)
        ADD_FACE(BlockTextureID::planks, LEFT_FACE)
        ADD_FACE(BlockTextureID::planks, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::red_flower:
        ADD_FACE(BlockTextureID::red_flower, UP_FACE)
        ADD_FACE(BlockTextureID::red_flower, DOWN_FACE)
        ADD_FACE(BlockTextureID::red_flower, FRONT_FACE)
        ADD_FACE(BlockTextureID::red_flower, BACK_FACE)
        ADD_FACE(BlockTextureID::red_flower, LEFT_FACE)
        ADD_FACE(BlockTextureID::red_flower, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::yellow_flower:
        ADD_FACE(BlockTextureID::yellow_flower, UP_FACE)
        ADD_FACE(BlockTextureID::yellow_flower, DOWN_FACE)
        ADD_FACE(BlockTextureID::yellow_flower, FRONT_FACE)
        ADD_FACE(BlockTextureID::yellow_flower, BACK_FACE)
        ADD_FACE(BlockTextureID::yellow_flower, LEFT_FACE)
        ADD_FACE(BlockTextureID::yellow_flower, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    default:
        return Geometry<TerrainVertex>{};
    }
}
