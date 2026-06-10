#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;
layout (location = 3) in float VertexBiome1;
layout (location = 4) in float VertexBiome2;
layout (location = 5) in float VertexBlendAlpha;

out vec3 FragPosition;
out vec3 vNormal;
out vec2 txtCoord;
out float height;
out vec3 vBiomeColor;

out vec3 vColor1;
out vec3 vColor2;
out float vAlpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 biome_color(int b, float h) {

    if (b == 0) { // PLAINS X
        float hn = clamp(h / 50.0, 0.0, 1.0);
        return mix(vec3(0.27, 0.40, 0.30), vec3(0.58, 0.67, 0.57), hn);
    }
    if (b == 1) { // MOUNTAINS X
        float hn = clamp(h / 100.0, 0.0, 1.0);
        return mix(vec3(0.16, 0.14, 0.11), vec3(0.96, 0.96, 0.88), hn);
    }
    if (b == 2) { // WATER X
        float hn = clamp((h + 30.0) / 30.0, 0.0, 1.0);
        return mix(vec3(0.14, 0.47, 0.62), vec3(0.51, 0.70, 0.76), hn);
    }
    if (b == 3) { // DESERT X
        float hn = clamp(h / 50.0, 0.0, 1.0);
        return mix(vec3(0.79, 0.47, 0.16), vec3(0.95, 0.65, 0.25), hn);
    }
    if (b == 4) { // MESA X
        float hn = clamp(h / 100.0, 0.0, 1.0);
        return mix(vec3(0.32, 0.07, 0.04), vec3(0.80, 0.35, 0.18), hn);
    }
    if (b == 5) { // WARM_WATER X
        float hn = clamp((h + 30.0) / 30.0, 0.0, 1.0);
        return mix(vec3(0.04, 0.67, 0.75), vec3(0.52, 0.90, 0.93), hn);
    }
    if (b == 6) { // SNOW X
        float hn = clamp(h / 50.0, 0.0, 1.0);
        return mix(vec3(0.80, 0.80, 0.95), vec3(0.95, 0.95, 1.00), hn);
    }
    if (b == 7) { // ICEBERG X
        float hn = clamp(h / 100.0, 0.0, 1.0);
        return mix(vec3(0.33, 0.60, 0.79), vec3(0.78, 0.91, 0.97), hn);
    }
    if (b == 8) { // COLD_WATER X
        float hn = clamp((h + 30.0) / 30.0, 0.0, 1.0);
        return mix(vec3(0.01, 0.17, 0.21), vec3(0.06, 0.41, 0.45), hn);
    }

    return vec3(1.0, 0.0, 1.0);
}

void main() {
    height = VertexPosition.y;
    txtCoord = VertexTexCoords;

    vColor1 = biome_color(int(VertexBiome1), height);
    vColor2 = biome_color(int(VertexBiome2), height);
    vAlpha = VertexBlendAlpha;

    vBiomeColor = mix(vColor1, vColor2, VertexBlendAlpha);

    FragPosition = vec3(model * vec4(VertexPosition, 1.0));
    vNormal = mat3(transpose(inverse(model))) * VertexNormal;

    gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
}