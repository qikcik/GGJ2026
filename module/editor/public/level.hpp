#pragma once
#include <memory>
#include <vector>

#include "actor.hpp"

class Level
{
public:
    std::vector<std::shared_ptr<ActorBase>> actors {};

    void saveToFile(std::string name,GameContext& ctx);
    void loadFromFile(std::string name,GameContext& ctx);
};
