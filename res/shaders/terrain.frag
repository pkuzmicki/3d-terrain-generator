#version 330 core

out vec4 FragColor;

in vec3 FragPosition;
in vec3 vNormal;
in vec2 txtCoord;
in float height;
in vec3 vBiomeColor;

in vec3 vColor1;
in vec3 vColor2;
in float vAlpha;

uniform sampler2D texture_diffuse1;

uniform bool useLight;

void main() {
    vec3 light_pos = vec3(0.0, 500.0, 0.0);
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float ambient_strength = 0.3;
    vec3 ambient = ambient_strength * light_color;

    vec3 n = normalize(vNormal);
    vec3 light_dir = normalize(light_pos - FragPosition);
    float d = max(dot(n, light_dir), 0.0);
    vec3 diffuse = d * light_color;

    //float a = smoothstep(0.0, 1.0, vAlpha);
    //vec3 c = (ambient + diffuse) * mix(vColor1, vColor2, a);

    vec3 color = useLight ? vBiomeColor * (ambient + diffuse) : vBiomeColor;
    
    FragColor = vec4(color, 1.0);
}