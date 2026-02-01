#pragma once
#include "actor.hpp"
#include "config.hpp"
#include "world.hpp"
#include "level.hpp"
#include <memory>
#include  <math/vec.hpp>

class GameplayMode;
class ReactiveActor;
class PlayerActor;
class QModelInstance;

class GameContext {
public:
    Config& config;
    World& world;
    Vec2i size {};
    float deltaTime = 1.f/60;
    Level level {};

    std::shared_ptr<PlayerActor>  player;
    std::vector<std::shared_ptr<ReactiveActor>> interactable;

    std::shared_ptr<GameplayMode> nextMode  {};
    std::shared_ptr<GameplayMode> mode  {};
};

