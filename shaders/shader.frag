#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in float index;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;

void main(){
    vec3 lightdir = normalize(vec3(1.0,10.0, 10.0));
    float diff = max( dot(normal, lightdir), 0.0);
    float ambient = 0.5;
    vec3 light = vec3(1.0) * (diff + ambient);
    float i = index;
    vec2 texOffset = vec2(0.0, 15.0);

    while(i>=16){
        texOffset.y += -1.0;
        i = i - 16.0;
    }
    
    if(abs(index - 3.0) < 0.001){
        if(abs(normal.y - 1.0) < 0.001){
            FragColor = texture(texture_diffuse1, (texCoord + texOffset) * vec2(1.0/16.0)) * vec4(0.6,0.9,0.4,1.0) * vec4(light, 1.0);
            return;
        }
        else if(abs(normal.y + 1.0) < 0.001)
            i = 2;   
    }
    texOffset.x += i;

    FragColor = texture(texture_diffuse1, (texCoord + texOffset) * vec2(1.0/16.0)) * vec4(light, 1.0);
}
