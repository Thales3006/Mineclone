#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <iostream>
#include <string>

class Texture{
    public:
        unsigned int ID;
        std::string type;
        std::string path;
        int width, height, nrChannels;

        void loadTex();
    public:
        Texture();
        Texture(std::string tipo, const char* imagePath);
        Texture(const Texture& other);

        ~Texture();

        void bind();
        void unbind();

        void changeTexture(std::string tipo, const char* imagePath);

        Texture& operator=(const Texture& other);
        
        std::string getType();
        std::string getPath();
};

#endif