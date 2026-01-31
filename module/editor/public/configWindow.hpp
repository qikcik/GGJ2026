#pragma once

#include "config.hpp"
#include "configHelper.hpp"
#include "window.hpp"
#include "reflection/serializeGuiPreview.hpp"

class ConfigWindow : public Window
{
public:
    explicit ConfigWindow(Config& config) : Window("ConfigWindow"), config(config)
    {
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
        config.onSerialize(&preview);

        if(ImGui::Button("Save")) {
            ConfigSave("config",config);
        }
        if(ImGui::Button("Load")) {
            ConfigLoad("config",config);
        }
        if(ImGui::Button("Default")) {
            config = Config();
        }
    }

protected:
    Config& config;
};
