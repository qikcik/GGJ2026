#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "window.hpp"
#include "profiler/profiler.hpp"

class WindowManager
{

public:
    static WindowManager* get();

    void queueAddWindowView(std::unique_ptr<Window> inView)
    {
        if(!inView) return;
        queuedWindowViews.push_back(std::move(inView));
    }

    void run(Vec2i inWinSize, std::function<void(void)> inPreGameloopStartCallback);

    struct
    {
        bool showImGuiShowcase = false;
        bool showImGuiDebugInfo = false;
    } properties {};

private:
    std::vector<std::unique_ptr<Window>> queuedWindowViews;
    std::vector<std::unique_ptr<Window>> windowViews;

    // Singleton Part
protected:
    WindowManager() = default;
    WindowManager(WindowManager &other) = delete;
    void operator=(const WindowManager &) = delete;

    static WindowManager* instance;
};

inline WindowManager* WindowManager::get()
{
    if(instance == nullptr)
        instance = new WindowManager();
    return instance;
}

inline void WindowManager::run(const Vec2i inWinSize, std::function<void(void)> inPreGameloopStartCallback)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI );
    InitWindow(inWinSize.x, inWinSize.y, "QEngine");
    rlImGuiSetup(true);
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
#ifdef IMGUI_HAS_DOCK
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    inPreGameloopStartCallback();

    while (!WindowShouldClose())
    {
        Profiler::get()->startCapture();

        {
            ScopeProfile("remove old windows");
            for(auto& it : queuedWindowViews) {
                windowViews.push_back(std::move(it));
                windowViews[windowViews.size() - 1]->onInstanced();
            }
            queuedWindowViews.clear();
        }

        std::erase_if(windowViews,
                      [](const std::unique_ptr<Window>& it) { return it->isPendingDestroy(); });

        for(auto& it : windowViews) {
            it->onUpdate( GetFrameTime() );
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
#ifdef IMGUI_HAS_DOCK
        ImGui::DockSpaceOverViewport();
#endif
        if (properties.showImGuiShowcase)
            ImGui::ShowDemoWindow();

        for(auto& it : windowViews)
            it->ImGuiDraw();
        rlImGuiEnd();

        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow();
}
