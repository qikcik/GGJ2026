#pragma once
#include <string>

#include "raymath.h"
#include "reflection/serialize.hpp"
#include "rlights.hpp"

class Config
{
public:
    Camera edCamera;
    float animationTime {0.25f};
    std::string test;
    bool grid = false;

    Config() {
        edCamera.position = (Vector3){ 25, 25.0f, 50 };
        edCamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        edCamera.up = (Vector3){ 0.0f, 1.f, 0.0f };
        edCamera.fovy = 25.0f;
        edCamera.projection = CAMERA_ORTHOGRAPHIC;
    }

    void onSerialize(ISerialize* inSerialize)
    {
        inSerialize->propertyFloat("camera_fov",edCamera.fovy);
        inSerialize->propertyBool("grid",grid);
        inSerialize->propertyFloat("animationTime",animationTime);
        bool value = edCamera.projection == CAMERA_ORTHOGRAPHIC;
        inSerialize->propertyBool("camera_ortographic",value);
        edCamera.projection = value ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE;
    }
};
