#pragma once
#include "gameContext.hpp"

class GameplayMode
{
public:
    GameplayMode(GameContext& ctx) : ctx(ctx) {};
    virtual ~GameplayMode() = default;

    virtual void onDraw2D() {};
    virtual void onUpdate() {};
protected:
    GameContext& ctx;
};
