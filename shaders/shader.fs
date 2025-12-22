#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

in float height;

uniform sampler2D outTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

    float h = clamp((height - 20.0) / (70.0 - 20.0), 0.0, 1.0);

    vec3 darkGreen  = vec3(0.0, 0.15, 0.05);
    vec3 lightGreen = vec3(0.8, 1.0, 0.8);

    vec3 color = mix(darkGreen, lightGreen, h);


    FragColor = vec4(color, 1.0);
    //FragColor = texture(outTexture, TexCoord) * vec4(ourColor, 1.0);
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    //FragColor = texture(texture1, TexCoord);
}