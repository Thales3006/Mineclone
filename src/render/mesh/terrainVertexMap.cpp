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

    plantBlock = Geometry<TerrainVertex>{
        {
            {glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)},
            {glm::vec3(1, 0, 1), glm::vec3(0, 0, 1)},
            {glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)},
            {glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)},

            {glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)},
            {glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)},
            {glm::vec3(0, 1, 1), glm::vec3(0, 0, -1)},
            {glm::vec3(1, 1, 0), glm::vec3(0, 0, -1)},
        },
        {0, 1, 3, 1, 2, 3, 6, 7, 4, 6, 4, 5,
         0, 3, 1, 1, 3, 2, 6, 4, 7, 6, 5, 4},
    };

    auto normalize_xz = [](glm::vec3 v) {
        glm::vec3 center(0.5f, 0.0f, 0.5f);
        return center +
               (v - center) / glm::vec3(std::sqrt(2.0f), 1.0f, std::sqrt(2.0f));
    };
    for (auto &vertex : plantBlock.vertices) {
        vertex.position = normalize_xz(vertex.position);
    }
}

Geometry<TerrainVertex>
TerrainVertexMap::setUV(Geometry<TerrainVertex> geometry, BlockTextureID id) {
    std::array<glm::vec2, 4> tex = textureManager->getBlockUV(id);
    for (int i = 0; i < geometry.vertices.size(); i++) {
        geometry.vertices[i].uv = tex[i % 4];
    }
    return geometry;
}

#define ADD_FACE(TEXTURE_ID, FACE)                                             \
    if (block.faces & FACE)                                                    \
        blockGeometry.append(setUV(fullBlock[FACE], TEXTURE_ID));

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
    case BlockID::grass_block:
        ADD_FACE(BlockTextureID::dirt, DOWN_FACE)
        ADD_FACE(BlockTextureID::grass_block_side, FRONT_FACE)
        ADD_FACE(BlockTextureID::grass_block_side, BACK_FACE)
        ADD_FACE(BlockTextureID::grass_block_side, LEFT_FACE)
        ADD_FACE(BlockTextureID::grass_block_side, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        if (block.faces & UP_FACE) {
            auto geometry =
                setUV(fullBlock[UP_FACE], BlockTextureID::grass_block_top);
            for (auto &vertex : geometry.vertices)
                vertex.color = glm::vec3(0.5f, 0.8, 0.4f);
            blockGeometry.append(geometry);
        }
        return blockGeometry;
    case BlockID::bricks:
        ADD_FACE(BlockTextureID::bricks, UP_FACE)
        ADD_FACE(BlockTextureID::bricks, DOWN_FACE)
        ADD_FACE(BlockTextureID::bricks, FRONT_FACE)
        ADD_FACE(BlockTextureID::bricks, BACK_FACE)
        ADD_FACE(BlockTextureID::bricks, LEFT_FACE)
        ADD_FACE(BlockTextureID::bricks, RIGHT_FACE)
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
    case BlockID::oak_planks:
        ADD_FACE(BlockTextureID::oak_planks, UP_FACE)
        ADD_FACE(BlockTextureID::oak_planks, DOWN_FACE)
        ADD_FACE(BlockTextureID::oak_planks, FRONT_FACE)
        ADD_FACE(BlockTextureID::oak_planks, BACK_FACE)
        ADD_FACE(BlockTextureID::oak_planks, LEFT_FACE)
        ADD_FACE(BlockTextureID::oak_planks, RIGHT_FACE)
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::red_flower:
        blockGeometry = setUV(plantBlock, BlockTextureID::red_flower);
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::yellow_flower:
        blockGeometry = setUV(plantBlock, BlockTextureID::yellow_flower);
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(1.0f);
        return blockGeometry;
    case BlockID::grass:
        blockGeometry = setUV(plantBlock, BlockTextureID::grass);
        for (auto &vertex : blockGeometry.vertices)
            vertex.color = glm::vec3(0.5f, 0.6, 0.4f);
        return blockGeometry;
    default:
        return Geometry<TerrainVertex>{};
    }
}
