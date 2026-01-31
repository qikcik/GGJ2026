#pragma once
#include <memory>
#include <vector>

//#include "graphic3d/meshInstance.hpp"

class QModelInstance;

class Level
{
public:
    std::vector<std::shared_ptr<QModelInstance>> instances {};

    void saveToFile(std::string name);
    void loadFromFile(std::string name);
};
