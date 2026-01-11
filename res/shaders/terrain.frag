#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

in float height;

uniform sampler2D texture_diffuse1;
uniform bool useTexture;

void main() {

    float h = clamp((height) / (100.0), 0.0, 1.0);

    vec3 darkGreen  = vec3(0.0, 0.1, 0.0);
    vec3 lightGreen = vec3(0.9, 1.0, 0.9);

    vec3 color = useTexture ?
				texture(texture_diffuse1, TexCoord).rgb :
				mix(darkGreen, lightGreen, h);

    FragColor = vec4(color, 1.0);
}