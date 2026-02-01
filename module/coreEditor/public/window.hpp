#pragma once

#include <string>

#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include "math/vec.hpp"

class Window
{
public:
    explicit Window(std::string title) : title(std::move(title)) {}
    virtual ~Window() = default;

    virtual void onInstanced() {};
    virtual void onUpdate(float deltaTime) {}

    bool isPendingDestroy() const {return pendingDestroy;};
    bool isFocused() const {return focused;};
    std::string getTitle() const {return title;};

    void ImGuiDraw()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        bool open {true};
        if (ImGui::Begin(title.c_str(), &open /*ImGuiWindowFlags_NoScrollbar*/))
        {
            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
            ImVec2 vMax = ImGui::GetWindowContentRegionMax();
            Vec2i currentSize = Vec2i{(int)(vMax.x-vMin.x),(int)(vMax.y-vMin.y)};

            if(size.x != currentSize.x || size.y != currentSize.y)
            {
                auto oldSize = size;
                size = currentSize;
                onResize(oldSize,currentSize);
            }


            focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            onImGuiDraw();
        }
        if(!open)
        {
            closeRequested();
        }


        ImGui::End();
        ImGui::PopStyleVar();
    }

protected:
    friend class WindowManager;
    bool pendingDestroy = false;
    bool focused {false};
    Vec2i size {640,480};
    std::string title {};

    virtual void onResize(Vec2i oldSize, Vec2i newSize) {};
    virtual void onImGuiDraw() {};
    virtual void closeRequested() {};
};
