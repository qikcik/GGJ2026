#pragma once
#include "gameContext.hpp"
#include "gameplayMode.hpp"
#include "actors/playerActor.hpp"
#include "../windows/gameWindow.hpp"
#include "dialogMode2.hpp"

class DialogMode : public GameplayMode
{
public:
    DialogMode(GameContext& ctx, std::shared_ptr<ReactiveActor> other) : GameplayMode(ctx),other(other) {
        options.clear();
        for (auto it : other->states[other->state].dialogOptions)
            if (it.canPreform(ctx))
                options.push_back(it);
    };

    void onUpdate() override {
        dialogCounter += ctx.deltaTime;
        animCounter += ctx.deltaTime;

        if (IsKeyPressed(KEY_W)) {
            selected--;
            animCounter = 0;
        }
        if (IsKeyPressed(KEY_S)) {
            selected++;
            animCounter = 0;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            if(options[selected].giveTag != "")
                ctx.world.tags.push_back(options[selected].giveTag);

            ctx.nextMode = std::make_shared<DialogMode2>(ctx,options[selected]);
        }
        if (selected <0) selected = 0;
        if (selected>=options.size()) selected=options.size()-1;
    }

    void onDraw2D() override {
        auto& c = other->states[other->state];
        float speed = 30.f;
        auto height = 24*3+32;
        auto text_box = (Rectangle){ 16, 16, float(ctx.size.x-32), 24*3+8 };
        auto text_inner = (Rectangle){ 24, 24, float(ctx.size.x-32-16), 24*3 };
        DrawRectangleGradientV((int)text_box.x, (int)text_box.y, (int)text_box.width, (int)text_box.height, WHITE, LIGHTGRAY);
        DrawRectangleLinesEx(text_box, 4, BLACK);
        DrawTextBoxed(GetFontDefault(),TextSubtext(c.initialMsg.c_str(),0,dialogCounter/(1.f/speed)),text_inner,24,1,true,BLACK);

        if(dialogCounter/(1.f/speed) >= c.initialMsg.size()) {
            for (int i = 0; i!= options.size(); i++) {
                auto& option = options[i];
                auto str = std::to_string(i+1) + "." + options[i].label;

                float selectedModifer = i == selected ? -sin(animCounter*4)*4-16 : 0;

                auto text_box = (Rectangle){ ctx.size.x/2.f+selectedModifer, height+i*48.f, float(ctx.size.x/2-16), 24+16 };
                auto text_inner = (Rectangle){ ctx.size.x/2.f+8+selectedModifer, height+i*48.f+8, float(ctx.size.x/2-16), 24+8 };
                DrawRectangleGradientV((int)text_box.x+2, (int)text_box.y+2, (int)text_box.width-4, (int)text_box.height-4, WHITE, WHITE);
                DrawRectangleLinesEx(text_box, 4, BLACK);

                if ( options[i].giveTag != "" || options[i].goToLevel != "")
                    DrawTextBoxed(GetFontDefault(),TextSubtext(options[i].label.c_str(),0,dialogCounter/(1.f/speed)),text_inner,24,1,true,RED);
                else
                    DrawTextBoxed(GetFontDefault(),TextSubtext(options[i].label.c_str(),0,dialogCounter/(1.f/speed)),text_inner,24,1,true,BLACK);

            }
        }
    };

protected:
    int selected=0;
    std::shared_ptr<ReactiveActor> actors;
    std::shared_ptr<ReactiveActor> other;
    std::vector<DialogOption> options;
    float dialogCounter = 0.f;
    float animCounter = 0.f;
};
