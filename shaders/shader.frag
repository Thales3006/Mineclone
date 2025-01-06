#version 460 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;

void main(){
    vec3 lightdir = normalize(vec3(1.0,10.0, 10.0));
    float diff = max( dot(normal, lightdir), 0.0);
    float ambient = 0.5;
    vec3 light = vec3(1.0) * (diff + ambient);

    FragColor = texture(texture_diffuse1, texCoord) * vec4(light, 1.0); //* texture(texture_diffuse2, texCoord);
}