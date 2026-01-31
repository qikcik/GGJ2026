#pragma once
#include "raylib.h"

class QShader
{
public:
    void load() {
        shader = LoadShader(TextFormat("shaders/lighting.vs", 330),TextFormat("shaders/lighting.fs", 330));
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
        ambientLoc = GetShaderLocation(shader, "ambient");
    }

public:
    int ambientLoc {};
    Shader shader {};
};
