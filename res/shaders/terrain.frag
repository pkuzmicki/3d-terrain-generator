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

    vec3 c;

    // if (h > 0.5) c = mix(mix(lightGreen, darkGreen, clamp(h, 0.0, 1.0)), mix(vec3(0.9, 1.0, 1.0), vec3(0.75, 0.75, 0.8), clamp(h, 0.0, 1.0)), h);
    // else mix(darkGreen, vec3(0.0, 0.45, 0.75), clamp(h, 0.0, 1.0));

    //c = mix(mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), clamp(h, 0.2, 1.0)), mix(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0), clamp(h, 0.2, 1.0)), h);
    c = mix(mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0), clamp(h, 0.2, 1.0)), mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), clamp(h, 0.2, 1.0)), h);

    
    // if (h > 0.8) c = mix(vec3(0.9, 1.0, 1.0), vec3(0.75, 0.75, 0.8), clamp(h, 0.0, 1.0));
    // else if (h < 0.8 && h > 0.4) c = mix(lightGreen, darkGreen, clamp(h, 0.0, 1.0));
    // else mix(darkGreen, vec3(0.0, 0.45, 0.75), clamp(h, 0.0, 1.0));

    vec3 color = useTexture ? texture(texture_diffuse1, TexCoord).rgb : c;

    FragColor = vec4(color, 1.0);
}