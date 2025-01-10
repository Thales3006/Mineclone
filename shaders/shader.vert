#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 anormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float ID;

out vec2 texCoord;
out vec3 normal;
out float index;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    texCoord = aTexCoord;
    normal = mat3(transpose(inverse(model)))*anormal;
    index = ID;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}