#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include "stb/stb_image.h"
#include <iostream>
#include <string>

class Texture{
    private:
        unsigned int ID;
        std::string type;
        std::string path;
    public:
        Texture(std::string tipo, const char* imagePath);

        void bind();

        void changeTexture(std::string tipo, const char* imagePath);

        unsigned int getID();
        std::string getType();
        std::string getPath();
};

#endif