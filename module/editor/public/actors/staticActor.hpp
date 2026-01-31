#pragma once
#include "actor.hpp"
#include "gameContext.hpp"
#include "graphic3d/mesh.hpp"
#include "graphic3d/meshInstance.hpp"

class StaticActor : public ActorBase
{
public:
    ACTOR_BODY(StaticActor);

    StaticActor() = default;

    void onPlaced(GameContext &ctx) override {
        model.modelRef = ctx.world.getModel(model.modelRef.path);
    }

    void onDraw(GameContext &) override {
        model.position = pos;
        DrawModelEx(model.modelRef.model->model, model.position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);
    }

    BoundingBox getBoundingBox() override {
        return model.boundingBox();
    }

    void onSerialize(ISerialize *inSerialize, GameContext &context) override {
        ActorBase::onSerialize(inSerialize, context);
        model.onSerialize(inSerialize,context.world.modelNames());
    }

    QModelInstance model;
};
