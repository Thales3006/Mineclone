#include "render/textureManager.h"

TextureManager::TextureManager() {
    textures = {
        {"container_box", std::make_shared<Texture>("textures/container.jpg")},
        {"blocks", std::make_shared<Texture>("textures/blocks_01.png")},
    };
}

std::shared_ptr<Texture> TextureManager::getTexture(std::string key) {
    if (auto it = textures.find(key); it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

std::array<glm::vec2, 4> TextureManager::getBlockUV(BlockTextureID id) {
    glm::vec2 size = textures["blocks"]->getSize();
    glm::vec2 sizeCell = glm::vec2(1.0f / (size / 16.0f));
    auto getUV = [=](int x, int y) -> std::array<glm::vec2, 4> {
        return {
            glm::vec2(x, y) * sizeCell,
            glm::vec2(x + 1, y) * sizeCell,
            glm::vec2(x + 1, y + 1) * sizeCell,
            glm::vec2(x, y + 1) * sizeCell,
        };
    };

    switch (id) {
    case BlockTextureID::stone:
        return getUV(1, 15);
    case BlockTextureID::dirt:
        return getUV(2, 15);
    case BlockTextureID::grass_side:
        return getUV(3, 15);
    case BlockTextureID::grass_top:
        return getUV(0, 15);
    case BlockTextureID::brick_block:
        return getUV(7, 15);
    case BlockTextureID::tnt_side:
        return getUV(8, 15);
    case BlockTextureID::tnt_top:
        return getUV(9, 15);
    case BlockTextureID::tnt_bottom:
        return getUV(10, 15);
    case BlockTextureID::smooth_stone:
        return getUV(6, 15);
    case BlockTextureID::planks:
        return getUV(4, 15);
    case BlockTextureID::red_flower:
        return getUV(12, 15);
    case BlockTextureID::yellow_flower:
        return getUV(13, 15);
    default:
        return getUV(12, 1);
    }
}
