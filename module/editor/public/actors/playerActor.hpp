#pragma once
#include <algorithm>

#include "actor.hpp"
#include "gameContext.hpp"
#include "graphic3d/mesh.hpp"
#include "graphic3d/meshInstance.hpp"

class PlayerActor : public ActorBase
{
public:
    ACTOR_BODY(PlayerActor);

    PlayerActor() = default;

    void onUpdate(GameContext & ctx) override {
        int prevState = state;
        if (state >= ctx.config.playerStates.size())
            state = 0;

        bool assigned = false;
        for (int stateIdx = ctx.config.playerStates.size() - 1; stateIdx >= 0; --stateIdx)
        {
            auto& stateIt = ctx.config.playerStates[stateIdx];
            if (std::ranges::contains(ctx.world.tags, stateIt.requiredTag)) {
                state = stateIdx;
                assigned = true;
                break;
            }
        }
        if (!assigned) state = 0;


        if (prevState!= state)
            frame =0;

        time -= ctx.deltaTime;

        if (time <= 0.0f) {
            time = ctx.config.animationTime;
            frame+=1;
        }

        if (frame >= ctx.config.playerStates[state].frames.size())
            frame = 0;

        model = ctx.config.playerStates[state].frames[frame];

    }
    void onPlaced(GameContext &ctx) override {
        for (auto& itState : ctx.config.playerStates)
        {
            for (auto& it : itState.frames)
            {
                it.modelRef = ctx.world.getModel(it.modelRef.path);
            }
        }
        colisionModel =  ctx.config.playerStates[0].frames[0];
    }

    void onDraw(GameContext &) override {
        model.position = pos;
        DrawModelEx(model.modelRef.model->model, model.position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);
    }

    BoundingBox getBoundingBox() override {
        colisionModel.position = pos;
        return colisionModel.boundingBox();
    }

    void onSerialize(ISerialize *inSerialize, GameContext &context) override {
        ActorBase::onSerialize(inSerialize, context);
    }

    QModelInstance colisionModel;
    QModelInstance model;
    int state = 0;
    int frame = 0;
    float time =  0;
};
