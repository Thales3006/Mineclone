#version 330 core

layout (location = 0) in vec3 vaPosition;
layout (location = 1) in vec3 vaNormal;
layout (location = 2) in vec2 vaUV;
layout (location = 3) in vec3 vaColor;

out vec2 texCoord;
out vec3 normal;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    texCoord = vaUV;
    normal = mat3(transpose(inverse(model)))*vaNormal;
    color = vaColor;
    gl_Position = projection * view * model * vec4(vaPosition, 1.0);
}
