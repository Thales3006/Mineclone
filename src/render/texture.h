#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glm/vec2.hpp>
#include <string>

class Texture {
  private:
    unsigned int ID;
    std::string path;
    int width, height, nrChannels;

  public:
    Texture();
    Texture(const char *imagePath);

    void bind();

    unsigned int getID();
    std::string getType();
    std::string getPath();
    glm::uvec2 getSize();
};

#endif
