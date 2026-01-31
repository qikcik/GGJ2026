#pragma once
#include "config.hpp"
#include "world.hpp"
#include "level.hpp"

class QModelInstance;

class GameContext {
public:
    Config config;
    World world;
    Level level;
};

