#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 color;

uniform sampler2D block_tex;

void main(){
    vec3 lightdir = normalize(vec3(1.0,10.0, 10.0));
    float diff = max( dot(normal, lightdir), 0.0);
    float ambient = 0.5;
    vec3 light = vec3(1.0) * (diff + ambient);

    FragColor = texture(block_tex, texCoord)  * vec4(light, 1.0) * vec4(color, 1.0);

    if (FragColor.a < 0.1) {
      discard;
    }
}
