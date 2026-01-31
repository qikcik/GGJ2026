#pragma once
#include <string>

#include "mesh.hpp"
#include "raylib.h"
#include "raymath.h"
#include "reflection/serialize.hpp"

class QModelInstance {
public:
    QModelRef modelRef {};
    Vector3 position {};

    BoundingBox boundingBox() {
        return {
            Vector3Add(modelRef.model->boundingBox.min,position),
            Vector3Add(modelRef.model->boundingBox.max,position)
        };
    };

    void onSerialize(ISerialize* inSerialize,std::vector<std::string> models)
    {
        inSerialize->propertyStruct("position",[&](ISerialize* inSerialize) {
            inSerialize->propertyFloat("x",position.x);
            inSerialize->propertyFloat("y",position.y);
            inSerialize->propertyFloat("z",position.z);
        });

        inSerialize->propertyEnum("model",models,modelRef.path);
    }
};
