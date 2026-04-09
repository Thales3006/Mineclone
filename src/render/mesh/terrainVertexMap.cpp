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

Geometry<TerrainVertex> TerrainVertexMap::getGeometry(ChunkRegion &region,
                                                      int x, int y, int z) {
    Geometry<TerrainVertex> blockGeometry{};
    unsigned char faces = region.getBlockFaces(x, y, z);

    auto add_face = [this, &blockGeometry, &region, x, y, z,
                     faces](BlockTextureID texture_id, unsigned char face) {
        if (faces & face) {
            auto geometry = setUV(fullBlock[face], texture_id);
            auto occlusion = region.getBlockOcclusion(x, y, z, face);
            for (int i = 0; i < 4; i++) {
                geometry.vertices[i].color = glm::vec3(occlusion[i]);
            }
            blockGeometry.append(geometry);
        }
    };

    switch (region.main->blocks[x][y][z].id) {
    case BlockID::air:
        return Geometry<TerrainVertex>{};
    case BlockID::stone:
        add_face(BlockTextureID::stone, UP_FACE);
        add_face(BlockTextureID::stone, DOWN_FACE);
        add_face(BlockTextureID::stone, FRONT_FACE);
        add_face(BlockTextureID::stone, BACK_FACE);
        add_face(BlockTextureID::stone, LEFT_FACE);
        add_face(BlockTextureID::stone, RIGHT_FACE);
        return blockGeometry;
    case BlockID::dirt:
        add_face(BlockTextureID::dirt, UP_FACE);
        add_face(BlockTextureID::dirt, DOWN_FACE);
        add_face(BlockTextureID::dirt, FRONT_FACE);
        add_face(BlockTextureID::dirt, BACK_FACE);
        add_face(BlockTextureID::dirt, LEFT_FACE);
        add_face(BlockTextureID::dirt, RIGHT_FACE);
        return blockGeometry;
    case BlockID::grass_block:
        if (faces & UP_FACE) {
            auto geometry =
                setUV(fullBlock[UP_FACE], BlockTextureID::grass_block_top);
            auto occlusion = region.getBlockOcclusion(x, y, z, UP_FACE);
            for (int i = 0; i < 4; i++)
                geometry.vertices[i].color =
                    glm::vec3(0.5f, 0.8f, 0.4f) * occlusion[i];
            blockGeometry.append(geometry);
        }
        add_face(BlockTextureID::dirt, DOWN_FACE);
        add_face(BlockTextureID::grass_block_side, FRONT_FACE);
        add_face(BlockTextureID::grass_block_side, BACK_FACE);
        add_face(BlockTextureID::grass_block_side, LEFT_FACE);
        add_face(BlockTextureID::grass_block_side, RIGHT_FACE);
        return blockGeometry;
    case BlockID::bricks:
        add_face(BlockTextureID::bricks, UP_FACE);
        add_face(BlockTextureID::bricks, DOWN_FACE);
        add_face(BlockTextureID::bricks, FRONT_FACE);
        add_face(BlockTextureID::bricks, BACK_FACE);
        add_face(BlockTextureID::bricks, LEFT_FACE);
        add_face(BlockTextureID::bricks, RIGHT_FACE);
        return blockGeometry;
    case BlockID::tnt:
        add_face(BlockTextureID::tnt_top, UP_FACE);
        add_face(BlockTextureID::tnt_bottom, DOWN_FACE);
        add_face(BlockTextureID::tnt_side, FRONT_FACE);
        add_face(BlockTextureID::tnt_side, BACK_FACE);
        add_face(BlockTextureID::tnt_side, LEFT_FACE);
        add_face(BlockTextureID::tnt_side, RIGHT_FACE);
        return blockGeometry;
    case BlockID::smooth_stone:
        add_face(BlockTextureID::smooth_stone, UP_FACE);
        add_face(BlockTextureID::smooth_stone, DOWN_FACE);
        add_face(BlockTextureID::smooth_stone, FRONT_FACE);
        add_face(BlockTextureID::smooth_stone, BACK_FACE);
        add_face(BlockTextureID::smooth_stone, LEFT_FACE);
        add_face(BlockTextureID::smooth_stone, RIGHT_FACE);
        return blockGeometry;
    case BlockID::oak_planks:
        add_face(BlockTextureID::oak_planks, UP_FACE);
        add_face(BlockTextureID::oak_planks, DOWN_FACE);
        add_face(BlockTextureID::oak_planks, FRONT_FACE);
        add_face(BlockTextureID::oak_planks, BACK_FACE);
        add_face(BlockTextureID::oak_planks, LEFT_FACE);
        add_face(BlockTextureID::oak_planks, RIGHT_FACE);
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
            vertex.color = glm::vec3(0.5f, 0.6f, 0.4f);
        return blockGeometry;
    default:
        return Geometry<TerrainVertex>{};
    }
}

Geometry<TerrainVertex> TerrainVertexMap::getSelectionBox() {
    Geometry<TerrainVertex> blockGeometry{};

    auto add_face = [this, &blockGeometry](BlockTextureID texture_id,
                                           unsigned char face) {
        auto geometry = setUV(fullBlock[face], texture_id);
        for (int i = 0; i < 4; i++) {
            geometry.vertices[i].color = glm::vec3(0.2f);
        }
        blockGeometry.append(geometry);
    };

    add_face(BlockTextureID::stone, UP_FACE);
    add_face(BlockTextureID::stone, DOWN_FACE);
    add_face(BlockTextureID::stone, FRONT_FACE);
    add_face(BlockTextureID::stone, BACK_FACE);
    add_face(BlockTextureID::stone, LEFT_FACE);
    add_face(BlockTextureID::stone, RIGHT_FACE);
    return blockGeometry;
}
