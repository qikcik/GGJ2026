#include <fstream>
#include <raylib.h>

#include "configWindow.hpp"
#include "windowManager.hpp"
#include "gameWindow.hpp"
#include "profilerWindow.hpp"

#include "configHelper.hpp"
#include "editorContext.hpp"
#include "selectedWindow.hpp"
#include "actors/staticActor.hpp"

WindowManager*  WindowManager::instance = nullptr;
Profiler*  Profiler::instance = nullptr;

int main(void)
{
    GameContext gameContext;
    EditorContext editorContext;

    gameContext.actorFactory.entries.push_back({
        StaticActor::ClassName,
        [](){return std::make_unique<StaticActor>();}
    });

    ConfigLoad("config",gameContext.config);
    InitAudioDevice();

    ChangeDirectory("assets");

    WindowManager::get()->properties.showImGuiShowcase    = false;
    WindowManager::get()->properties.showImGuiDebugInfo   = false;


    WindowManager::get()->queueAddWindowView(std::make_unique<GameWindow>(gameContext,editorContext));
    WindowManager::get()->queueAddWindowView(std::make_unique<SelectedWindow>(gameContext,editorContext));
    WindowManager::get()->queueAddWindowView(std::make_unique<ConfigWindow>(gameContext.config));
    //WindowManager::get()->queueAddWindowView(std::make_unique<ProfilerWindow>());

    WindowManager::get()->run(
        Vec2i{1800,800},
        []
        {
            SetExitKey(KEY_F12);
        }
    );
    CloseAudioDevice();

    return 0;
}