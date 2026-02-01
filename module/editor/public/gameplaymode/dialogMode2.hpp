#pragma once
#include "gameContext.hpp"
#include "gameplayMode.hpp"
#include "actors/playerActor.hpp"
#include "../windows/gameWindow.hpp"

class DialogMode2 : public GameplayMode
{
public:
    DialogMode2(GameContext& ctx, DialogOption option) : GameplayMode(ctx),option(option) {

    };

    void onUpdate() override {
        dialogCounter += ctx.deltaTime;
        animCounter += ctx.deltaTime;

        if (IsKeyPressed(KEY_SPACE)) {
            if(option.goToLevel != "")
                goToLevel(ctx,option.goToLevel);

            goToPlayMode(ctx);
        }
    }

    void onDraw2D() override {
        float speed = 30.f;
        auto height = 24*3+32;
        auto text_box = (Rectangle){ 16, 16, float(ctx.size.x-32), 24*3+8 };
        auto text_inner = (Rectangle){ 24, 24, float(ctx.size.x-32-16), 24*3 };
        DrawRectangleGradientV((int)text_box.x, (int)text_box.y, (int)text_box.width, (int)text_box.height, WHITE, LIGHTGRAY);
        DrawRectangleLinesEx(text_box, 4, BLACK);
        DrawTextBoxed(GetFontDefault(),TextSubtext(option.message.c_str(),0,dialogCounter/(1.f/speed)),text_inner,24,1,true,BLACK);

        if(dialogCounter/(1.f/speed) >= option.message.size()) {
                float selectedModifer =  -sin(animCounter*4)*4-16 ;

                auto text_box = (Rectangle){ ctx.size.x/2.f+selectedModifer, height+0*48.f, float(ctx.size.x/2-16), 24+16 };
                auto text_inner = (Rectangle){ ctx.size.x/2.f+8+selectedModifer, height+0*48.f+8, float(ctx.size.x/2-16), 24+8 };
                DrawRectangleGradientV((int)text_box.x+2, (int)text_box.y+2, (int)text_box.width-4, (int)text_box.height-4, WHITE, WHITE);
                DrawRectangleLinesEx(text_box, 4, BLACK);

                DrawTextBoxed(GetFontDefault(),TextSubtext(option.exitMessage.c_str(),0,dialogCounter/(1.f/speed)),text_inner,24,1,true,BLACK);

            }
    }

protected:
    DialogOption option {};
    float dialogCounter = 0.f;
    float animCounter = 0.f;
};
