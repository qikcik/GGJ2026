#pragma once

#include <cmath>
#include <iostream>

#include "editorContext.hpp"
#include "gameContext.hpp"
#include "raymath.h"
#include "rlights.hpp"
#include "textureRenderer.hpp"
#include "window.hpp"
#include "world.hpp"
#include "graphic3d/meshInstance.hpp"

class EditorContext;
class World;

std::vector<std::string> loadFromFolder(std::string path)
{
    std::vector<std::string> result;
    auto dir = LoadDirectoryFilesEx((path).c_str(),".vox",true);
    for (int idx = 0; idx != dir.count; idx++)
    {
        std::cout<< dir.paths[idx];
        result.push_back( dir.paths[idx]);
    }
    UnloadDirectoryFiles(dir);
    return result;
}

class GameWindow : public Window
{
public:
    explicit GameWindow(GameContext& gameCtx, EditorContext& editorCtx) : Window("GameWindow"),
        gameCtx(gameCtx), editorCtx(editorCtx) {
    }

    ~GameWindow()
    {
    }

    void onInstanced() override
    {
        auto& world = gameCtx.world;
        auto& level = gameCtx.level;
        auto& shader = gameCtx.world.shader;
        shader.load();

        for(auto& it : loadFromFolder("models"))
        {
            world.models.push_back(std::make_shared<QModel>(it));
        }

        for (const auto& it : world.models)
            it->loadModel(world.shader);

        float i = -16;
        for (auto it : world.models)
        {
            i += 4;
            level.instances.push_back(std::make_shared<QModelInstance>(it,Vector3{i*1.f,0,0}));
        }
        gameCtx.config.light = CreateLight(LIGHT_DIRECTIONAL, (Vector3) { -1, 1, 0.4f }, (Vector3) { 0, 0, 0 }, GRAY, world.shader.shader);

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
};

inline void GameWindow::onUpdate(float deltaTime) {
    auto _ = ScopeProfile("onUpdate");

    auto& world = gameCtx.world;
    auto& level = gameCtx.level;
    auto& shader = gameCtx.world.shader;
    auto& config = gameCtx.config;
    auto& camera = gameCtx.config.edCamera;

    Vec2f mousePos;
    mousePos.x = (GetMousePosition().x - renderer.lastRenderedScreenRect.x);
    mousePos.y = (GetMousePosition().y - renderer.lastRenderedScreenRect.y);


    float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
    SetShaderValue(world.shader.shader, world.shader.shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
    SetShaderValue(world.shader.shader, world.shader.ambientLoc, (float[4]) { config.ambient, config.ambient, config.ambient, 1.0f }, SHADER_UNIFORM_VEC4);
    UpdateLightValues(world.shader.shader, config.light);

    if (isFocused() && mousePos.x >= 0 &&  mousePos.y >= 0 && mousePos.x < renderer.lastRenderedScreenRect.width && mousePos.y < renderer.lastRenderedScreenRect.height ) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            auto ray = GetScreenToWorldRayEx(Vector2{mousePos.x,mousePos.y}, camera, renderer.lastRenderedScreenRect.width, renderer.lastRenderedScreenRect.height);
            RayCollision collision = { 0 };
            collision.distance = FLT_MAX;
            collision.hit = false;
            std::shared_ptr<QModelInstance> selectedModel {};
            for (auto it : gameCtx.level.instances)
            {
                RayCollision boxHitInfo = GetRayCollisionBox(ray, it->boundingBox());
                std::cout << boxHitInfo.hit << std::endl;
                if ((boxHitInfo.hit) && (boxHitInfo.distance < collision.distance)) {
                    collision.distance = boxHitInfo.distance;
                    collision.hit = true;
                    selectedModel = it;
                }
            }
            editorCtx.selectedModel = selectedModel;
        }


        if (auto p = editorCtx.selectedModel.lock()) {
            if (counter < 0) {
                if (IsKeyDown(KEY_A))
                    p->position.x -= 0.25;
                if (IsKeyDown(KEY_D))
                    p->position.x += 0.25;
                if (IsKeyDown(KEY_W))
                    p->position.z -= 0.25;
                if (IsKeyDown(KEY_S))
                    p->position.z += 0.25;
                counter = (1.f/GetFrameTime())/20;
            }
            counter--;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            UpdateCamera(&camera,CAMERA_FIRST_PERSON);
    }

    renderer.drawContent([&]()
    {
        BeginMode3D(camera);

        //DrawSphereEx(config.light.position, 0.2f, 8, 8, config.light.color);
        //DrawLine3D(config.light.position,config.light.target, config.light.color);
        DrawGrid(1000,2.f);
        for(auto& it : gameCtx.level.instances) {
            DrawModelEx(it->modelRef.model->model, it->position, Vector3{0,1,0},it->rotation, Vector3(1.f,1.f,1.f), WHITE);
        }

        if (auto p = editorCtx.selectedModel.lock()) {
            DrawBoundingBox(p->boundingBox(), RED );
        }

        EndMode3D();
        DrawFPS(10,10);
    });
}
