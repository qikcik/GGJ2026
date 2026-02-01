#pragma once

#include "gameContext.hpp"
#include "../config.hpp"
#include "../configHelper.hpp"
#include "window.hpp"
#include "reflection/serializeGuiPreview.hpp"

class TagsWindow : public Window
{
public:
    explicit TagsWindow(GameContext& ctx) : Window("TagsWindow"), ctx(ctx)
    {
    }

    ~TagsWindow()
    {
    }

    void onInstanced() override
    {

    }

    void onUpdate(float deltaTime) override
    {

    }

    void onImGuiDraw() override {
        SerializeGuiPreview inSerialize;

        int tagNum = ctx.world.tags.size();
        inSerialize.propertyInt("num",tagNum);
        if (tagNum<0) tagNum = 0;
        if ( ctx.world.tags.size() != tagNum)
            ctx.world.tags.resize(tagNum);

        for(int i = 0; i < tagNum ; i++)
        {
            inSerialize.propertyString(std::to_string(i+1),ctx.world.tags[i]);
        }

        ImGui::SeparatorText("Level Settings");
        ctx.level.onSerialize(&inSerialize);
        ImGui::SeparatorText("actions");
        if (ImGui::Button("Save Current Camera")) {
            ctx.level.cameraPosition = ctx.config.edCamera.position;
            ctx.level.cameraTarget = ctx.config.edCamera.target;
            ctx.level.fovy = ctx.config.edCamera.fovy;
        }
        if (ImGui::Button("Set Game Camera")) {
            ctx.config.edCamera.position = ctx.level.cameraPosition;
            ctx.config.edCamera.target = ctx.level.cameraTarget;
            ctx.config.edCamera.fovy = ctx.level.fovy;
        }
    }

protected:
    GameContext& ctx;
};
