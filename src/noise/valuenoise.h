#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

#define s_curve(t) ((t)*(t)*(3.0f - 2.0f*(t)))
#define lerp(t, a, b) (a + (t)*(b - a))

class ValueNoiseGeneration {
private:
    static const int SIZE = 256;
    static const int MASK = 0xFF;

    float position[SIZE];
    int permute[SIZE];

    float noise(float x, float z) {
        //const int dx = (int)x;
        const int dx = (int)std::floor(x);
        const float sx = s_curve(x - dx);
        const int i = permute[MASK & dx];
        const int j = permute[MASK & (dx + 1)];

        //const int dz = (int)z;
        const int dz = (int)std::floor(z);
        const float sz = s_curve(z - dz);

        float u0 = position[permute[(i + dz) & MASK]];
        float u1 = position[permute[(j + dz) & MASK]];
        const float v0 = lerp(sx, u0, u1);
        u0 = position[permute[(i + dz + 1) & MASK]];
        u1 = position[permute[(j + dz + 1) & MASK]];
        const float v1 = lerp(sx, u0, u1);

        return lerp(sz, v0, v1);
    }

public:
    void Initialize() {
        for (int i = 0; i < SIZE; i++) {
            permute[i] = i;   
        }

        for (int i = SIZE-1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = permute[i];
            permute[i] = permute[j];
            permute[j] = tmp;
        }
    }

    void SetValueTable(int table[], const int n) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += table[i];
        }
        if (sum != SIZE) {
            std::cout<<"Height distribution values must sum to "<<SIZE<<", not "<<sum<<"\n";
            return;
        }

        float delta = 2.0f/(float)(n-1);
        float min = -1.0f;
        int k = 0;
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < table[i]; j++) {
                position[k++] = min + delta * (float)rand()/(float)RAND_MAX;
            }
            min += delta; 
        }

        for (int j = 0; j < table[n-1]; j++) {
            position[k++] = 1.0f;
        }
    }

    float GetHeight(float x, float z, float a, float b, int n) {
        float result = 0.0f;
        float scale = 1.0f;

        for (int i = 0; i < n; i++) {
            result += scale * noise(x, z);
            scale *= a;
            x *= b;
            z *= b;
        }

        return (1.0f + result * 1.414213f * (a - 1.0f) / (scale - 1.0f)) / 2.0f;
    }
    
};