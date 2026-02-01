#pragma once

#include <cmath>
#include <iostream>

#include "../actorHelper.hpp"
#include "../editorContext.hpp"
#include "../gameContext.hpp"
#include "raymath.h"
#include "../rlights.hpp"
#include "textureRenderer.hpp"
#include "window.hpp"
#include "../world.hpp"
#include "actors/playerActor.hpp"
#include "graphic3d/meshInstance.hpp"

class EditorContext;
class World;

class EditorWindow : public Window
{
public:
    explicit EditorWindow(GameContext& gameCtx, EditorContext& editorCtx) : Window("EditorWindow"),
        gameCtx(gameCtx), editorCtx(editorCtx) {
    }

    ~EditorWindow()
    {
    }

    void onInstanced() override
    {
        auto& world = gameCtx.world;
        auto& level = gameCtx.level;
        auto& shader = gameCtx.world.shader;

        renderer.changeSize({512, 512});

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
    GameContext& gameCtx;
    EditorContext& editorCtx;
    int counter = 0;

    Matrix lightView = { 0 };
    Matrix lightProj = { 0 };
    Matrix lightViewProj = { 0 };
    int textureActiveSlot = 10;
};

inline void EditorWindow::onUpdate(float deltaTime) {
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


    if (isFocused() && mousePos.x >= 0 &&  mousePos.y >= 0 && mousePos.x < renderer.lastRenderedScreenRect.width && mousePos.y < renderer.lastRenderedScreenRect.height ) {
        if (IsKeyPressed(KEY_SPACE)) {
            if (auto p = editorCtx.selectedModel.lock()) {
                std::shared_ptr<ActorBase> copy = duplicateActor(p.get(),gameCtx);

                level.actors.push_back(copy);
                editorCtx.selectedModel = copy;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            auto ray = GetScreenToWorldRayEx(Vector2{mousePos.x,mousePos.y}, camera, renderer.lastRenderedScreenRect.width, renderer.lastRenderedScreenRect.height);
            RayCollision collision = { 0 };
            collision.distance = FLT_MAX;
            collision.hit = false;
            std::shared_ptr<ActorBase> selected {};
            for (auto it : gameCtx.level.actors)
            {
                RayCollision boxHitInfo = GetRayCollisionBox(ray, it->getBoundingBox());
                std::cout << boxHitInfo.hit << std::endl;
                if ((boxHitInfo.hit) && (boxHitInfo.distance < collision.distance)) {
                    collision.distance = boxHitInfo.distance;
                    collision.hit = true;
                    selected = it;
                }
            }
            editorCtx.selectedModel = selected;
        }


        if (auto p = editorCtx.selectedModel.lock()) {

            if (IsKeyDown(KEY_LEFT_CONTROL))
            {
                if (IsKeyPressed(KEY_A))
                    p->pos.x -= p->getBoundingBox().max.x - p->getBoundingBox().min.x;
                if (IsKeyPressed(KEY_D))
                    p->pos.x += p->getBoundingBox().max.x - p->getBoundingBox().min.x;
                if (IsKeyPressed(KEY_W))
                    p->pos.z -= p->getBoundingBox().max.z - p->getBoundingBox().min.z;
                if (IsKeyPressed(KEY_S))
                    p->pos.z += p->getBoundingBox().max.z - p->getBoundingBox().min.z;

                if (IsKeyPressed(KEY_R))
                    p->pos.y += p->getBoundingBox().max.y - p->getBoundingBox().min.y;
                if (IsKeyPressed(KEY_F))
                    p->pos.y -= p->getBoundingBox().max.y - p->getBoundingBox().min.y;
            }
            else
            {
                if (IsKeyPressed(KEY_A))
                    counter = 0;
                if (IsKeyPressed(KEY_D))
                    counter = 0;
                if (IsKeyPressed(KEY_W))
                    counter = 0;
                if (IsKeyPressed(KEY_S))
                    counter = 0;

                if (IsKeyPressed(KEY_R))
                    counter = 0;
                if (IsKeyPressed(KEY_F))
                    counter = 0;

                if (counter <= 0) {
                    if (IsKeyDown(KEY_A))
                        p->pos.x -= 0.25;
                    if (IsKeyDown(KEY_D))
                        p->pos.x += 0.25;
                    if (IsKeyDown(KEY_W))
                        p->pos.z -= 0.25;
                    if (IsKeyDown(KEY_S))
                        p->pos.z += 0.25;

                    if (IsKeyDown(KEY_R))
                        p->pos.y += 0.25;
                    if (IsKeyDown(KEY_F))
                        p->pos.y -= 0.25;
                    counter = (1.f/GetFrameTime())/ (IsKeyDown(KEY_LEFT_SHIFT) ? 40.f : 10.f);
                }
                counter--;
            }

            if (IsKeyPressed(KEY_DELETE)) {
                std::erase_if(level.actors, [&](auto el) { return el == p;});
            }

        }



        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            UpdateCamera(&camera,CAMERA_FIRST_PERSON);
    }

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
            if (auto p = editorCtx.selectedModel.lock()) {
                DrawBoundingBox(p->getBoundingBox(), RED );
            }
        EndMode3D();
        DrawFPS(10,10);
    });
}
