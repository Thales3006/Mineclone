#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture(){
    ID = 0;
    width = 0;
    height = 0;
    nrChannels =0;
    path = "";
    type = "";
}

Texture::Texture(std::string tipo, const char* imagePath){
    path = imagePath;
    type = tipo;
    ID = 0;

    loadTex();
}

Texture::Texture(const Texture& other){
    *this = other;
}

Texture::~Texture() {
    if(ID) glDeleteTextures(1,&ID);
    ID = 0;
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture& Texture::operator=(const Texture& other){
    this->path = other.path;
    this->type = other.type;

    if(ID) glDeleteTextures(1,&ID);
    ID = 0;

    loadTex();
    
    return *this;
}

void Texture::changeTexture(std::string tipo, const char* imagePath){
    *this = Texture(tipo, imagePath);
}

std::string Texture::getType(){
    return type;
}

std::string Texture::getPath(){
    return path;
}

void Texture::loadTex(){
    if(path == "" || type == "")
        return;

    if(!ID)
        glGenTextures(1, &ID);
    else{
        glDeleteTextures(1, &ID);
        glGenTextures(1, &ID);
    }
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);  
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Image loading error" << std::endl; 
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}