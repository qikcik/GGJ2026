#pragma once

#include "../config.hpp"
#include "../configHelper.hpp"
#include "window.hpp"
#include "reflection/serializeGuiPreview.hpp"

class ConfigWindow : public Window
{
public:
    explicit ConfigWindow(Config& config,World& world) : Window("ConfigWindow"), config(config), world_(world) {
    }

    ~ConfigWindow()
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
        SerializeGuiPreview preview;
        ImGui::SeparatorText("config");
        config.onSerialize(&preview,world_);

        ImGui::Separator();

        ImGui::SeparatorText("config action");
        if(ImGui::Button("Save")) {
            ConfigSave("config",config,world_);
        }
        ImGui::SameLine();
        if(ImGui::Button("Load")) {
            ConfigLoad("config",config,world_);
        }
        ImGui::SameLine();
        if(ImGui::Button("Default")) {
            config = Config();
        }
    }

protected:
    Config& config;
    World &world_;
};
