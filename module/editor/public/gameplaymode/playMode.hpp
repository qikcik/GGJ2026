#pragma once
#include "dialogMode.hpp"
#include "gameContext.hpp"
#include "gameplayMode.hpp"
#include "actors/playerActor.hpp"
#include "gameOver.hpp"

class PlayMode : public GameplayMode
{
public:
    PlayMode(GameContext& ctx) : GameplayMode(ctx) {};

    void onDraw2D() override {};
    void onUpdate() override {

        if (IsKeyPressed(KEY_A))
            counter = 0;
        if (IsKeyPressed(KEY_D))
            counter = 0;
        if (IsKeyPressed(KEY_W))
            counter = 0;
        if (IsKeyPressed(KEY_S))
            counter = 0;

        if (counter <= 0) {
            auto prevPos = ctx.player->pos;

            if (IsKeyDown(KEY_A))
                ctx.player->pos.x -= 0.25;
            if (IsKeyDown(KEY_D))
                ctx.player->pos.x += 0.25;
            if (IsKeyDown(KEY_W))
                ctx.player->pos.z -= 0.25;
            if (IsKeyDown(KEY_S))
                ctx.player->pos.z += 0.25;

            auto playerBox = ctx.player->getBoundingBox();
            playerBox.min.y += 1;
            std::shared_ptr<ActorBase> other {};
            for (auto it : ctx.level.actors) {
                if ( ctx.player != it && CheckCollisionBoxes(playerBox,it->getBoundingBox()))
                    other = it;
            }

            if(other) {
                ctx.player->pos = prevPos;
                if (auto p = std::dynamic_pointer_cast<ReactiveActor>(other))
                if (p->states[p->state].interactable) {
                    ctx.nextMode = std::make_shared<DialogMode>(ctx,p);
                }
            }

            counter = (1.f/ctx.deltaTime) / 30.f;
        }
        counter--;
    }



protected:
    int counter {0};
};
