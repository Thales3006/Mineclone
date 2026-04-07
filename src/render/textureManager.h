#ifndef TEXTURE_MANAGER_CLASS_H
#define TEXTURE_MANAGER_CLASS_H

#include "render/texture.h"
#include "world/terrain/block/block.h"

#include <array>
#include <glm/vec2.hpp>
#include <map>
#include <memory>
#include <string>

enum class BlockTextureID {
    stone,
    dirt,
    grass_side,
    grass_top,
    brick_block,
    tnt_side,
    tnt_top,
    tnt_bottom,
    smooth_stone,
    planks,
    red_flower,
    yellow_flower,
};

class TextureManager {
  private:
    std::map<std::string, std::shared_ptr<Texture>> textures;

  public:
    TextureManager();

    std::shared_ptr<Texture> getTexture(std::string key);

    std::array<glm::vec2, 4> getBlockUV(BlockTextureID id);
};

#endif
