#pragma once

#include <functional>

#include "math/vec.hpp"
#include "windowManager.hpp"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"


class TextureRenderer {
public:
    bool textureLoaded {false};
    RenderTexture texture {};
    Rectangle     lastRenderedScreenRect {0,0,1,1};

    ~TextureRenderer() {
        if(textureLoaded)
            UnloadRenderTexture(texture);
    }


    void changeSize(Vec2i inSize) {
        if(textureLoaded)
            UnloadRenderTexture(texture);
        texture = LoadRenderTexture(inSize.x, inSize.y);
        textureLoaded = true;
    }

    Vec2i getTextureSize() const {
        if(textureLoaded)
            return {texture.texture.width,texture.texture.height};
        return {0,0};
    }

    void drawContent(const std::function<void()>& inDrawCallback, const Color inColor = DARKGRAY)
    {
        if(!textureLoaded || texture.texture.width == 0)
            return;

        BeginTextureMode(texture);
        ClearBackground(inColor);
        inDrawCallback();
        EndTextureMode();
    }


    RenderTexture* getRenderTexture()
    {
        if(textureLoaded)
            return &texture;
        return nullptr;
    }

    Texture2D* getTexture()
    {
        if(!textureLoaded || texture.texture.width == 0)
            return nullptr;

        return &texture.texture;
    }

    void draw()
    {
        rlImGuiImageRenderTexture(&texture);
        afterImGuiDraw_updateRenderScreenRect();
    }

    void drawFit()
    {
        rlImGuiImageRenderTextureFit(&texture,false);
        afterImGuiDraw_updateRenderScreenRect();
    }

    void afterImGuiDraw_updateRenderScreenRect()
    {
        lastRenderedScreenRect = {
            ImGui::GetItemRectMin().x,
            ImGui::GetItemRectMin().y,
            ImGui::GetItemRectMax().x-ImGui::GetItemRectMin().x,
            ImGui::GetItemRectMax().y-ImGui::GetItemRectMin().y,
        };
        if(WindowManager::get()->properties.showImGuiDebugInfo)
            ImGui::GetForegroundDrawList()->AddRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32( 255, 255, 0, 255 ) );
    }

};
