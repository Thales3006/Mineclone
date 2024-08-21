#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
    private:
        unsigned int ID;
    public:
        Shader(const char* vertexFilePath, const char* fragmentFilePath);

        void use();

        void setBool(const std::string &name, bool value);  
        void setInt(const std::string &name, int value);   
        void setFloat(const std::string &name, float value);
        void setMat4(const std::string &name, glm::mat4 value);


    private:
        std::string readFile(const char* filePath);
        void checkCompileErrors(unsigned int shader, std::string type);
};

#endif