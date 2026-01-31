#pragma once

#include <cmath>
#include <iostream>

#include "editorContext.hpp"
#include "raymath.h"
#include "textureRenderer.hpp"
#include "window.hpp"
#include "world.hpp"
#include "graphic3d/meshInstance.hpp"
#include "reflection/serializeGuiPreview.hpp"

class EditorContext;
class World;

class SelectedWindow : public Window
{
public:
    explicit SelectedWindow(GameContext& gameContext, EditorContext& editorContext) : Window("SelectedWindow"),
        gameCtx(gameContext), editorCtx(editorContext) {
    }

    ~SelectedWindow()
    {
    }

    void onInstanced() override
    {

    }

    void onUpdate(float deltaTime) override
    {

    }

    std::string level_name {"playground"};

    void onImGuiDraw() override
    {
        ImGui::Separator();

        SerializeGuiPreview preview;
        preview.propertyString("level_name",level_name);

        if(ImGui::Button("Save")) {
            gameCtx.level.saveToFile(level_name);
        }
        ImGui::SameLine();
        if(ImGui::Button("Load")) {
            gameCtx.level.loadFromFile(level_name);
            for (auto it : gameCtx.level.instances)
                it->modelRef = gameCtx.world.getModel(it->modelRef.path);
        }

        ImGui::Separator();

        if(auto p = editorCtx.selectedModel.lock())
        {
            SerializeGuiPreview preview;
            p->onSerialize(&preview,gameCtx.world.modelNames());
            p->modelRef = gameCtx.world.getModel(p->modelRef.path);
        }
    }


protected:
    GameContext& gameCtx;
    EditorContext& editorCtx;
    int counter = 0;
};

