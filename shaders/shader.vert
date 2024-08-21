#version 460 core

#define PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    texCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}