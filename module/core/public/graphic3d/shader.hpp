#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

class QShader
{
public:
    void load() {
        shader = LoadShader(TextFormat("shaders/shadowmap.vs", 330),TextFormat("shaders/shadowmap.fs", 330));

        Vector3 lightDir = Vector3Normalize((Vector3){ 0.35f, -1.0f, -0.35f });
        Color lightColor = WHITE;
        Vector4 lightColorNormalized = ColorNormalize(lightColor);
        int lightDirLoc = GetShaderLocation(shader, "lightDir");
        int lightColLoc = GetShaderLocation(shader, "lightColor");
        SetShaderValue(shader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
        int ambientLoc = GetShaderLocation(shader, "ambient");
        float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
        SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
        int shadowMapResolution = SHADOWMAP_RESOLUTION;
        SetShaderValue(shader, GetShaderLocation(shader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);

        shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);
    }

    static RenderTexture2D LoadShadowmapRenderTexture(int width, int height)
    {
        RenderTexture2D target = { 0 };

        target.id = rlLoadFramebuffer(); // Load an empty framebuffer
        target.texture.width = width;
        target.texture.height = height;

        if (target.id > 0)
        {
            rlEnableFramebuffer(target.id);

            target.depth.id = rlLoadTextureDepth(width, height, false);
            target.depth.width = width;
            target.depth.height = height;
            target.depth.format = 19; // DEPTH_COMPONENT_24BIT?
            target.depth.mipmaps = 1;

            rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

            if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] ShadowMap Framebuffer object created successfully", target.id);
            rlDisableFramebuffer();
        }
        else TRACELOG(LOG_WARNING, "FBO: ShadowMap Framebuffer object can not be created");

        return target;
    }

public:
    Shader shader {};

    const int SHADOWMAP_RESOLUTION = 4096;
    RenderTexture2D shadowMap {};
};
