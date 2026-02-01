#include <fstream>
#include <raylib.h>

#include "windowManager.hpp"
#include "public/windows/editorWindow.hpp"
#include "profilerWindow.hpp"

#include "configHelper.hpp"
#include "editorContext.hpp"
#include "selectedWindow.hpp"
#include "actors/reactiveActor.hpp"
#include "actors/staticActor.hpp"
#include "actors/simpleAnimationActor.hpp"
#include "windows/configWindow.hpp"
#include "windows/gameWindow.hpp"
#include "windows/tagsWindow.hpp"

WindowManager*  WindowManager::instance = nullptr;
Profiler*  Profiler::instance = nullptr;


std::vector<std::string> loadFromFolder(std::string path)
{
    std::vector<std::string> result;
    auto dir = LoadDirectoryFilesEx((path).c_str(),".vox",true);
    for (int idx = 0; idx != dir.count; idx++)
    {
        std::cout<< dir.paths[idx];
        std::string path = dir.paths[idx];
        std::replace(path.begin(), path.end(), '\\', '/');
        result.push_back( path);
    }
    UnloadDirectoryFiles(dir);
    return result;
}

int main(void)
{
    InitAudioDevice();

    Config config;
    World world;

    world.actorFactory.entries.push_back({
        StaticActor::ClassName,
        [](){return std::make_unique<StaticActor>();}
    });

    world.actorFactory.entries.push_back({
        SimpleAnimationActor::ClassName,
        [](){return std::make_unique<SimpleAnimationActor>();}
    });

    world.actorFactory.entries.push_back({
        ReactiveActor::ClassName,
        [](){return std::make_unique<ReactiveActor>();}
    });

    world.actorFactory.entries.push_back({
        PlayerActor::ClassName,
        [](){return std::make_unique<PlayerActor>();}
    });

    ChangeDirectory("assets");
    ConfigLoad("config",config);

    GameContext gameContext {config,world};
    EditorContext editorContext;

    WindowManager::get()->properties.showImGuiShowcase    = false;
    WindowManager::get()->properties.showImGuiDebugInfo   = false;


    WindowManager::get()->queueAddWindowView(std::make_unique<EditorWindow>(gameContext,editorContext));
    WindowManager::get()->queueAddWindowView(std::make_unique<SelectedWindow>(gameContext,editorContext));
    WindowManager::get()->queueAddWindowView(std::make_unique<ConfigWindow>(gameContext.config));
    WindowManager::get()->queueAddWindowView(std::make_unique<TagsWindow>(gameContext));
    //WindowManager::get()->queueAddWindowView(std::make_unique<ProfilerWindow>());

    WindowManager::get()->run(
        Vec2i{1800,800},
        [&]
        {
            SetExitKey(KEY_F12);

            gameContext.world.shader.load();

            for(auto& it : loadFromFolder("models"))
            {
                gameContext.world.models.push_back(std::make_shared<QModel>(it));
            }

            for (const auto& it : gameContext.world.models)
                it->loadModel(gameContext.world.shader);
        }
    );
    CloseAudioDevice();

    return 0;
}