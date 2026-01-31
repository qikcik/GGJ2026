#pragma once

#include <cmath>
#include <iostream>
#include "config.hpp"
#include "textureRenderer.hpp"
#include "window.hpp"
#include "reflection/serializeGuiPreview.hpp"

class ProfilerWindow : public Window
{
public:
    explicit ProfilerWindow() : Window("ProfilerWindow")
    {
    }

    ~ProfilerWindow()
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
        for(auto& entry : Profiler::get()->getPreviousTrace())
            if ( entry.type == ProfileEntryType::Start)
                ImGui::Text("%i %s %s",std::chrono::duration_cast<std::chrono::milliseconds>(entry.time.time_since_epoch()), entry.type == ProfileEntryType::Start ? "Start" : "Stop",entry.name);
        //SerializeGuiPreview preview;
        //config.onSerialize(&preview);
    }

};
