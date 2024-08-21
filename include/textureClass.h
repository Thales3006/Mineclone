#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include"stb/stb_image.h"

class Texture{
    public:
        unsigned int ID;
    public:
        Texture(const char* imagePath){
            glGenTextures(1,&ID);
            glBindTexture(GL_TEXTURE_2D, ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);  
            unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
            if(data){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else{
                std::cout << "Erro ao carregar imagem." << std::endl; 
            }

            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void bind(){
            glBindTexture(GL_TEXTURE_2D, ID);
        }
};

#endif