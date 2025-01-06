#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader{
    private:
        unsigned int ID;
    public:
        Shader();
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