#pragma once
#include "actor.hpp"
#include "gameContext.hpp"
#include "graphic3d/meshInstance.hpp"

class SimpleAnimationActor : public ActorBase
{
public:
    ACTOR_BODY(SimpleAnimationActor);

    SimpleAnimationActor() = default;

    void onPlaced(GameContext &ctx) override {
        frame1.modelRef = ctx.world.getModel(frame1.modelRef.path);
        frame2.modelRef = ctx.world.getModel(frame2.modelRef.path);
    }

    void onUpdate(GameContext &ctx) override {
        time -= ctx.deltaTime;

        if (time <= 0.0f) {
            time = ctx.config.animationTime;
            frame = !frame;
        }
    }

    void onDraw(GameContext &) override {
        if (frame) {
            frame1.position = pos;
            DrawModelEx(frame1.modelRef.model->model, frame1.position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);
        }
        else {
            frame2.position = pos;
            DrawModelEx(frame2.modelRef.model->model, frame2.position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);
        }
    }

    BoundingBox getBoundingBox() override {
        return frame1.boundingBox();
    }

    void onSerialize(ISerialize *inSerialize, GameContext &context) override {
        ActorBase::onSerialize(inSerialize, context);
        inSerialize->propertyStruct("frame1",[&](ISerialize *inSerialize) {
            frame1.onSerialize(inSerialize,context.world.modelNames());
        });
        inSerialize->propertyStruct("frame2",[&](ISerialize *inSerialize) {
            frame2.onSerialize(inSerialize,context.world.modelNames());
        });
    }

    QModelInstance frame1;
    QModelInstance frame2;

    float time = 0.0f;
    bool frame = true;
};
