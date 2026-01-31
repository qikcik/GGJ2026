#pragma once
#include <memory>
#include <vector>

#include "graphic3d/mesh.hpp"
#include "graphic3d/shader.hpp"

class QModel;
class QModelInstance;

class World
{
public:
    std::vector<std::shared_ptr<QModel>> models {};
    QShader shader {};

    std::vector<std::string> modelNames()
    {
        std::vector<std::string>  result;
        for (auto it : models)
        {
            result.push_back(it->path);
        }
        return result;
    };

    QModelRef getModel(const std::string& inPath)
    {
        std::string path = inPath;
        std::replace(path.begin(), path.end(), '\\', '/');
        std::vector<std::string>  result;
        for (auto it : models)
        {
            if (it->path == path)
            {
                return {it};
            }
        }
        return getModel("models/AAA.vox");
    };
};
