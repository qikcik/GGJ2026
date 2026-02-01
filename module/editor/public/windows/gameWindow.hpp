#pragma once

#include <cmath>
#include <iostream>

#include "helper.hpp"
#include "../actorHelper.hpp"
#include "../editorContext.hpp"
#include "../gameContext.hpp"
#include "raymath.h"
#include "../rlights.hpp"
#include "textureRenderer.hpp"
#include "window.hpp"
#include "../world.hpp"
#include "actors/playerActor.hpp"
#include "actors/reactiveActor.hpp"
#include "graphic3d/meshInstance.hpp"

class PlayerActor;
class EditorContext;
class World;

enum class Mode {
    Normal,
    Dialog
};

class GameWindow : public Window
{
public:
    explicit GameWindow(GameContext& inGameCtx,const std::string& level_name) : Window("GameWindow"), gameCtx{inGameCtx.config,inGameCtx.world}
    {
        gameCtx.level.loadFromFile(level_name,gameCtx);
    }

    ~GameWindow()
    {
    }

    void closeRequested() override {
        pendingDestroy = true;
    }

    void onInstanced() override
    {
        auto& world = gameCtx.world;
        auto& level = gameCtx.level;
        auto& shader = gameCtx.world.shader;
        auto& config = gameCtx.config;

        config.edCamera.position = level.cameraPosition;
        config.edCamera.target = level.cameraTarget;
        config.edCamera.fovy = level.fovy;

        renderer.changeSize({512, 512});

        for (auto it : level.actors) {
            if (auto p = std::dynamic_pointer_cast<PlayerActor>(it))
                player = p;
            if (auto p = std::dynamic_pointer_cast<ReactiveActor>(it))
                interactable.push_back(p);
        }

        if (player == nullptr) {
            pendingDestroy = true;
        }

    }

    void onUpdate(float deltaTime) override;

    void onImGuiDraw() override
    {
        renderer.draw();
    }

    void onResize(Vec2i oldSize, Vec2i newSize) override
    {
        renderer.changeSize(newSize);
    }

protected:
    TextureRenderer renderer;
    Texture2D viewportTexture;
    GameContext gameCtx;
    int counter = 0;

    Matrix lightView = { 0 };
    Matrix lightProj = { 0 };
    Matrix lightViewProj = { 0 };
    int textureActiveSlot = 10;

    std::shared_ptr<PlayerActor>  player;
    std::vector<std::shared_ptr<ReactiveActor>> interactable;

    Mode mode {Mode::Normal};
    std::shared_ptr<ReactiveActor> interactWith {};
    std::vector<DialogOption> options;
};

inline void GameWindow::onUpdate(float deltaTime) {
    gameCtx.deltaTime = deltaTime;
    auto _ = ScopeProfile("onUpdate");

    auto& world = gameCtx.world;
    auto& level = gameCtx.level;
    auto& shader = gameCtx.world.shader;
    auto& config = gameCtx.config;
    auto& camera = gameCtx.config.edCamera;

    shader.setLight(level.ambient,level.light,level.lightDir);

    Vector3 cameraPos = camera.position;
    SetShaderValue(shader.shader, shader.shader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);

    Vec2f mousePos;
    mousePos.x = (GetMousePosition().x - renderer.lastRenderedScreenRect.x);
    mousePos.y = (GetMousePosition().y - renderer.lastRenderedScreenRect.y);


    //logic here
    if (mode == Mode::Normal) {
        if (counter <= 0) {
            auto prevPos = player->pos;

            if (IsKeyDown(KEY_A))
                player->pos.x -= 0.25;
            if (IsKeyDown(KEY_D))
                player->pos.x += 0.25;
            if (IsKeyDown(KEY_W))
                player->pos.z -= 0.25;
            if (IsKeyDown(KEY_S))
                player->pos.z += 0.25;

            auto playerBox = player->getBoundingBox();
            std::shared_ptr<ReactiveActor> other {};
            for (auto it : interactable) {
                if ( CheckCollisionBoxes(playerBox,it->getBoundingBox()))
                    other = it;
            }

            if(other) {
                player->pos = prevPos;
                if (other->states[other->state].interactable) {
                    interactWith = other;
                    options.clear();
                    for (auto it : interactWith->states[interactWith->state].dialogOptions)
                        if (it.canPreform(gameCtx))
                            options.push_back(it);
                    mode = Mode::Dialog;
                }
            }

            counter = (1.f/gameCtx.deltaTime) / 30.f;
        }
        counter--;
    }
    if (mode == Mode::Dialog) {
        if (IsKeyPressed(KEY_ZERO))
            mode = Mode::Normal;
        if (IsKeyPressed(KEY_ONE))
        {
            if(options[0].giveTag != "")
                gameCtx.world.tags.push_back(options[0].giveTag);
            if(options[0].goToLevel != "")
                WindowManager::get()->queueAddWindowView(std::make_unique<GameWindow>(gameCtx,options[0].goToLevel ));
            mode = Mode::Normal;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            if(options[1].giveTag != "")
                gameCtx.world.tags.push_back(options[1].giveTag);
            if(options[1].goToLevel != "")
                WindowManager::get()->queueAddWindowView(std::make_unique<GameWindow>(gameCtx,options[1].goToLevel ));
            mode = Mode::Normal;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            if(options[2].giveTag != "")
                gameCtx.world.tags.push_back(options[1].giveTag);
            if(options[2].goToLevel != "")
                WindowManager::get()->queueAddWindowView(std::make_unique<GameWindow>(gameCtx,options[2].goToLevel ));
            mode = Mode::Normal;
        }
    }


    //game

    for(auto& it : gameCtx.level.actors) {
        it->onUpdate(gameCtx);
    }

    auto draw = [&]() {
        for(auto& it : gameCtx.level.actors) {
            it->onDraw(gameCtx);
        }
    };

    Vector3 lightDir = Vector3Normalize((Vector3){ 0.35f, -1.0f, -0.35f });
    Camera3D lightCamera = { 0 };
    lightCamera.position = Vector3Scale(Vector3Normalize(level.lightDir), -15.0f);
    lightCamera.target = Vector3Zero();
    lightCamera.projection = CAMERA_ORTHOGRAPHIC; // Use an orthographic projection for directional lights
    lightCamera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    lightCamera.fovy = 60.0f;
    //PASS.1
    BeginTextureMode(shader.shadowMap);
    ClearBackground(WHITE);

    BeginMode3D(lightCamera);
        lightView = rlGetMatrixModelview();
        lightProj = rlGetMatrixProjection();
        draw();
    EndMode3D();

    EndTextureMode();
    lightViewProj = MatrixMultiply(lightView, lightProj);

    //PASS.2
    renderer.drawContent([&]()
    {
        int lightVPLoc = GetShaderLocation(shader.shader, "lightVP");
        int shadowMapLoc = GetShaderLocation(shader.shader, "shadowMap");

        SetShaderValueMatrix(shader.shader, lightVPLoc, lightViewProj);
        rlEnableShader(shader.shader.id);

        rlActiveTextureSlot(textureActiveSlot);
        rlEnableTexture(shader.shadowMap.depth.id);
        rlSetUniform(shadowMapLoc, &textureActiveSlot, SHADER_UNIFORM_INT, 1);

        BeginMode3D(camera);
            if(config.grid)
                DrawGrid(100,1);
            draw();
        EndMode3D();

        if (mode == Mode::Dialog) {
            auto& c = interactWith->states[interactWith->state];
            DrawRectangleVoxelWithOutline((Rectangle){ 16, 16, float(size.x-32), 300 }, 0, 2, WHITE, BLACK);
            DrawText(c.initialMsg.c_str(),24,24,24,BLACK);

            for (int i = 0; i!= options.size(); i++) {
                auto str = std::to_string(i+1) + "." + options[i].label;

                if (options[i].giveTag != "")
                    DrawText(str.c_str(),24,24+32+(i*32),24,RED);
                else
                    DrawText(str.c_str(),24,24+32+(i*32),24,BLACK);
            }
        }
        DrawFPS(10,10);
    });
}