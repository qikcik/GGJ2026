#include "helper.hpp"

#include "windowManager.hpp"
#include "windows/gameWindow.hpp"

void goToLevel(GameContext& ctx, std::string level_name)
{
    WindowManager::get()->queueAddWindowView(std::make_unique<GameWindow>(ctx,level_name));
}

void goToPlayMode(GameContext& ctx)
{
    ctx.nextMode = std::make_shared<PlayMode>(ctx);
}