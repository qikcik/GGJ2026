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
#include "gameplaymode/gameplayMode.hpp"
#include "gameplaymode/playMode.hpp"
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
        gameCtx.mode = std::make_shared<PlayMode>(gameCtx);

        config.edCamera.position = level.cameraPosition;
        config.edCamera.target = level.cameraTarget;
        config.edCamera.fovy = level.fovy;

        renderer.changeSize({512, 512});

        for (auto it : level.actors) {
            if (auto p = std::dynamic_pointer_cast<PlayerActor>(it))
                gameCtx.player = p;
            if (auto p = std::dynamic_pointer_cast<ReactiveActor>(it))
                gameCtx.interactable.push_back(p);
        }

        if (gameCtx.player == nullptr) {
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
    float fadeCounter = 0.f;

    Matrix lightView = { 0 };
    Matrix lightProj = { 0 };
    Matrix lightViewProj = { 0 };
    int textureActiveSlot = 10;
};

inline void GameWindow::onUpdate(float deltaTime) {
    fadeCounter += deltaTime;
    if (gameCtx.nextMode) {
        gameCtx.mode = gameCtx.nextMode;
        gameCtx.nextMode = {};
    }
    gameCtx.deltaTime = deltaTime;
    gameCtx.size = size;
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


    //game
    gameCtx.mode->onUpdate();

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
            if ( gameCtx.player && gameCtx.config.debug) {
                auto box = gameCtx.player->getBoundingBox();
                box.min.y += gameCtx.config.colisionOffset;
                DrawBoundingBox(box,GREEN);
            }
        EndMode3D();
        gameCtx.mode->onDraw2D();

        if (fadeCounter < 1.0f) {
            DrawRectangle(0, 0, size.x, size.y, Fade(BLACK, 1.f-fadeCounter));
        }

        DrawFPS(10,10);
    });
}