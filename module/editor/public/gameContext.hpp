#pragma once
#include "actor.hpp"
#include "config.hpp"
#include "world.hpp"
#include "level.hpp"

class QModelInstance;

class GameContext {
public:
    Config& config;
    World& world;
    float deltaTime = 1.f/60;
    Level level {};


};

