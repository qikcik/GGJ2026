#pragma once
#include <algorithm>
#include <ranges>

#include "actor.hpp"
#include "gameContext.hpp"
#include "graphic3d/mesh.hpp"
#include "graphic3d/meshInstance.hpp"
#include "../gameContext.hpp"

struct DialogOption
{
    std::string label {};
    std::string message {};
    std::string exitMessage {"close"};
    std::string requiredTag {};

    std::string giveTag {};
    std::string goToLevel {};

    bool canPreform(GameContext &ctx) {
        if (!giveTag.empty() && std::ranges::contains(ctx.world.tags, giveTag)) return false;
        if (requiredTag.empty()) return true;
        return std::ranges::contains(ctx.world.tags, requiredTag);
    }
};

struct ReactiveActorState
{
    std::string requiredTag {"INVALID"};
    float timeModifier {1};
    std::vector<QModelInstance> frames {1};

    bool interactable = false;
    std::string initialMsg {};
    std::vector<DialogOption> dialogOptions {1};


    void onSerialize(ISerialize *inSerialize, GameContext &context) {
        inSerialize->propertyString("requiredTag",requiredTag);
        inSerialize->propertyFloat("timeModifier",timeModifier);
        int num = frames.size();
        inSerialize->propertyInt("framesNum",num);
        if (num<1) num = 1;
        if ( frames.size() != num)
            frames.resize(num);

        inSerialize->propertyStruct("frames",[&](ISerialize* inSerialize) {
            for(int i = 0; i < num ; i++)
            {
                inSerialize->propertyStruct(std::to_string(i),[&](ISerialize* inSerialize) {
                    frames[i].onSerialize(inSerialize,context.world.modelNames());
                });
            }
        });

        inSerialize->propertyBool("interactable",interactable);

        if(interactable)
        {
            inSerialize->propertyString("initialMsg",initialMsg);
            int num = dialogOptions.size();
            inSerialize->propertyInt("optionsNum",num);
            if (num<0) num = 0;
            if ( dialogOptions.size() != num)
                dialogOptions.resize(num);

            inSerialize->propertyStruct("option",[&](ISerialize* inSerialize) {
                for(int i = 0; i < num ; i++)
                {
                    inSerialize->propertyStruct(std::to_string(i),[&](ISerialize* inSerialize) {
                        inSerialize->propertyString("label",dialogOptions[i].label);
                        inSerialize->propertyString("message",dialogOptions[i].message);
                        inSerialize->propertyString("requiredTag",dialogOptions[i].requiredTag);
                        inSerialize->propertyString("giveTag",dialogOptions[i].giveTag);
                        inSerialize->propertyString("goToLevel",dialogOptions[i].goToLevel);
                    });
                }
            });
        }
    }
};

class ReactiveActor : public ActorBase
{
public:
    ACTOR_BODY(ReactiveActor);

    ReactiveActor() = default;

    void onPlaced(GameContext &ctx) override {
        for (auto& itState : states)
        {
            for (auto& it : itState.frames)
            {
                it.modelRef = ctx.world.getModel(it.modelRef.path);
            }
        }
    }

    void onUpdate(GameContext &ctx) override
    {
        int prevState = state;
        if (state >= states.size())
            state = 0;

        bool assigned = false;
        for (int stateIdx = states.size() - 1; stateIdx >= 0; --stateIdx)
        {
            auto& stateIt = states[stateIdx];
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
            time = ctx.config.animationTime * states[state].timeModifier;
            frame+=1;
        }

        if (frame >= states[state].frames.size())
            frame = 0;
    }
    void onDraw(GameContext &) override {
        auto& model = states[state].frames[frame];
        states[state].frames[frame].position = pos;
        DrawModelEx(model.modelRef.model->model, model.position, Vector3{0,1,0},0, Vector3(1.f,1.f,1.f), WHITE);

    }

    BoundingBox getBoundingBox() override {
        states[state].frames[0].position = pos;
        return states[state].frames[0].boundingBox();
    }

    void onSerialize(ISerialize *inSerialize, GameContext &ctx) override {
        ActorBase::onSerialize(inSerialize, ctx);

        int num = states.size();
        inSerialize->propertyInt("statesNum",num);
        if (num<1) num = 1;
        if ( states.size() != num)
            states.resize(num);

        states[0].requiredTag = "";

        inSerialize->propertyStruct("states",[&](ISerialize* inSerialize) {
            for(int i = 0; i < num ; i++)
            {
                inSerialize->propertyStruct(std::to_string(i),[&](ISerialize* inSerialize) {
                    states[i].onSerialize(inSerialize,ctx);
                });
            }
        });

    }

    int state = 0;
    int frame = 0;
    float time =  0;
    std::vector<ReactiveActorState> states {1};
};
