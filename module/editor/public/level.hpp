#pragma once
#include <memory>
#include <vector>

#include "actor.hpp"

class Level
{
public:
    std::vector<std::shared_ptr<ActorBase>> actors {};

    Vector3 cameraPosition = { 25, 25.0f, 50 };
    Vector3 cameraTarget = { 0.0f, 0.0f, 0.0f };
    float fovy = 25.0f;

    float ambient[4] = {};
    float light[4] = {};
    Vector3 lightDir = Vector3Normalize((Vector3){ 0.35f, -1.0f, -0.35f });;

    void onSerialize(ISerialize* inSerialize)
    {
        inSerialize->propertyStruct("light",[&](ISerialize* inSerialize) {
            inSerialize->propertyColor("ambient",ambient);
            inSerialize->propertyColor("light",light);
            inSerialize->propertyFloat("light.x",lightDir.x);
            inSerialize->propertyFloat("light.y",lightDir.y);
            inSerialize->propertyFloat("light.z",lightDir.z);
        });

        inSerialize->propertyStruct("camera",[&](ISerialize* inSerialize) {
            inSerialize->propertyFloat("position.x",cameraPosition.x);
            inSerialize->propertyFloat("position.y",cameraPosition.y);
            inSerialize->propertyFloat("position.z",cameraPosition.z);
            inSerialize->propertyFloat("target.x",cameraTarget.x);
            inSerialize->propertyFloat("target.y",cameraTarget.y);
            inSerialize->propertyFloat("target.z",cameraTarget.z);
            inSerialize->propertyFloat("fovy",fovy);
        });
    }

    void saveToFile(std::string name,GameContext& ctx);
    void loadFromFile(std::string name,GameContext& ctx);
};
