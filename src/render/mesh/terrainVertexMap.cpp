#include "render/mesh/terrainVertexMap.h"

#include "world/terrain/block/blockRegistry.h"

TerrainVertexMap::TerrainVertexMap(
    std::shared_ptr<TextureManager> textureManager)
    : textureManager(std::move(textureManager)) {
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

Geometry<TerrainVertex> TerrainVertexMap::getGeometry(const RegionSnapshot &region,
                                                      int x, int y, int z) {
    BlockID id = region.main->blocks[x][y][z].id;
    if (id == BlockID::air)
        return Geometry<TerrainVertex>{};

    const BlockDef &def = blockDef(id);

    if (def.model == BlockModel::Plant) {
        Geometry<TerrainVertex> geometry = setUV(plantBlock, def.faces[0]);
        for (auto &vertex : geometry.vertices)
            vertex.color = def.tint[0];
        return geometry;
    }

    static const unsigned char faceBits[6] = {FRONT_FACE, BACK_FACE, LEFT_FACE,
                                               RIGHT_FACE, UP_FACE, DOWN_FACE};

    unsigned char faces = region.getBlockFaces(x, y, z);
    Geometry<TerrainVertex> blockGeometry{};
    for (int i = 0; i < 6; i++) {
        if (!(faces & faceBits[i]))
            continue;

        Geometry<TerrainVertex> face =
            setUV(fullBlock.at(faceBits[i]), def.faces[i]);
        std::array<float, 4> occlusion =
            region.getBlockOcclusion(x, y, z, faceBits[i]);
        for (int v = 0; v < 4; v++)
            face.vertices[v].color = def.tint[i] * occlusion[v];
        blockGeometry.append(face);
    }
    return blockGeometry;
}

Geometry<TerrainVertex> TerrainVertexMap::getSelectionBox() {
    glm::vec2 uv = setUV(fullBlock.at(UP_FACE), BlockTextureID::stone).vertices[0].uv;

    const glm::vec3 corners[8] = {
        {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
        {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1},
    };

    const unsigned int edges[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, // bottom
        4, 5, 5, 6, 6, 7, 7, 4, // top
        0, 4, 1, 5, 2, 6, 3, 7, // verticals
    };

    Geometry<TerrainVertex> geometry{};
    for (const glm::vec3 &corner : corners)
        geometry.addVertex({corner, glm::vec3(0.0f), uv, glm::vec3(0.0f)});
    for (unsigned int index : edges)
        geometry.indices.push_back(index);

    return geometry;
}
