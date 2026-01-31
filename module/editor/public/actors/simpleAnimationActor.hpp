#pragma once
#include "actor.hpp"
#include "gameContext.hpp"
#include "graphic3d/meshInstance.hpp"

class SimpleAnimationActor : public ActorBase
{
public:
    ACTOR_BODY(SimpleAnimationActor);

    SimpleAnimationActor() : frames(2) {

    };

    void onPlaced(GameContext &ctx) override {
        for (auto& it : frames)
        {
            it.modelRef = ctx.world.getModel(it.modelRef.path);
        }
    }

    void onUpdate(GameContext &ctx) override {
        time -= ctx.deltaTime;

        if (time <= 0.0f) {
            time = ctx.config.animationTime * timeModifier;
            frame+=1;
        }

        if (frame >= frames.size())
            frame = 0;
    }

    void onDraw(GameContext &) override {
        frames[frame].position = pos;
        DrawModelEx(frames[frame].modelRef.model->model, frames[frame].position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);

    }

    BoundingBox getBoundingBox() override {
        return frames[frame].boundingBox();
    }

    void onSerialize(ISerialize *inSerialize, GameContext &context) override {
        ActorBase::onSerialize(inSerialize, context);
        inSerialize->propertyFloat("timeModifier",timeModifier);
        inSerialize->propertyInt("frameNum",frameNum);
        if (frameNum<1) frameNum = 1;
        if (frames.size() < frameNum)
            frames.resize(frameNum);

        for(int i = 0; i < frameNum ; i++)
        {
            inSerialize->propertyStruct("frame"+std::to_string(i+1),[&](ISerialize *inSerialize) {
                frames[i].onSerialize(inSerialize,context.world.modelNames());
            });
        }
    }

    int frameNum = 2;
    float timeModifier = 1.f;
    int frame = 0;
    std::vector<QModelInstance> frames;

    float time = 0.0f;
};
