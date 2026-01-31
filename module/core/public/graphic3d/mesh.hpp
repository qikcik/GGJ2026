#pragma once
#include <memory>
#include <string>

#include "raylib.h"
#include "shader.hpp"

class QModel
{
public:
    QModel(std::string in_path) : path {std::move(in_path)}, model {} {};

    void loadModel(const QShader& shader)
    {
        model = LoadModel(path.c_str());
        for (int j = 0; j < model.materialCount; j++)
            model.materials[j].shader = shader.shader;
        boundingBox = GetModelBoundingBox(model);
    }

public:
    std::string path {};
    Model model{};
    BoundingBox boundingBox {};
};

class QModelRef
{
public:
    QModelRef() = default;
    QModelRef(const std::shared_ptr<QModel> &in_model) {
        model = in_model;
        path = model->path;
    }

    std::string path {};
    std::shared_ptr<QModel> model {};
};