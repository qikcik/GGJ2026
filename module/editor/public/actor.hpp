#pragma once

#define ACTOR_BODY(className) \
static constexpr std::string ClassName = #className; \
std::string getClassName() override {return ClassName;}
#include <memory>

#include "raylib.h"
#include "raymath.h"
#include "reflection/serialize.hpp"


class GameContext;

class ActorBase
{
public:
    virtual ~ActorBase() = default;

    virtual std::string getClassName() = 0;

    virtual void onSerialize(ISerialize* inSerialize,GameContext& context)
    {
        inSerialize->propertyFloat("pos.x",pos.x);
        inSerialize->propertyFloat("pos.y",pos.y);
        inSerialize->propertyFloat("pos.z",pos.z);
    };

    virtual void onPlaced(GameContext&) = 0;
    virtual void onUpdate(GameContext&) {};
    virtual void onDraw(GameContext&) = 0;
    virtual BoundingBox getBoundingBox() = 0;

    Vector3 pos {};
};

struct ActorFactoryEntry
{
    std::string className;
    std::function<std::unique_ptr<ActorBase>()> construct;
};


struct ActorFactory
{
    ActorFactoryEntry* getByClassName(const std::string& inName)
    {
        for (auto& it : entries)
        {
            if (it.className == inName)
                return &it;
        }
        return nullptr;
    }

    std::vector<const char*> getNames() {
        std::vector<const char*> items;
        for (auto& it : entries)
        {
            items.push_back(it.className.c_str());
        }
        return items;
    }

    std::vector<ActorFactoryEntry> entries;
};