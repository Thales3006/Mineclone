#ifndef TEXTURE_MANAGER_CLASS_H
#define TEXTURE_MANAGER_CLASS_H

#include "render/texture.h"
#include "world/terrain/block/block.h"

#include <array>
#include <glm/vec2.hpp>
#include <map>
#include <memory>
#include <string>

class TextureManager {
  private:
    std::map<std::string, std::shared_ptr<Texture>> textures;

  public:
    TextureManager();

    std::shared_ptr<Texture> getTexture(const std::string &key);

    std::array<glm::vec2, 4> getBlockUV(BlockTextureID id);
};

#endif
