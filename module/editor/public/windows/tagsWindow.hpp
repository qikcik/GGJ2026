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

    void onImGuiDraw() override
    {
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
    }

protected:
    GameContext& ctx;
};
