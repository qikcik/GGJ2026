#pragma once
#include <string>

#include "raymath.h"
#include "reflection/serialize.hpp"
#include "rlights.hpp"
#include "world.hpp"
#include "graphic3d/meshInstance.hpp"

struct PlayerState
{
    std::vector<QModelInstance> frames {1};
    std::string requiredTag {"INVALID"};

    void onSerialize(ISerialize *inSerialize, World &world) {
        inSerialize->propertyString("requiredTag",requiredTag);
        int num = frames.size();
        inSerialize->propertyInt("framesNum",num);
        if (num<1) num = 1;
        if ( frames.size() != num)
            frames.resize(num);


        inSerialize->propertyStruct("frames",[&](ISerialize* inSerialize) {
            for(int i = 0; i < num ; i++)
            {
                inSerialize->propertyStruct("frame:"+std::to_string(i),[&](ISerialize* inSerialize) {
                    frames[i].onSerialize(inSerialize,world.modelNames());
                });
            }
        });
    }
};

class Config
{
public:
    Camera edCamera;
    float animationTime {0.25f};
    std::string test;
    bool grid = false;
    std::string startingLevel = "1";
    std::string gameoverText = "GAMEOVER - tym razem sie nie udalo, ale wiem wiecej";
    std::string gameoverRetryText = "sprobuj jeszcze raz";

    std::string preserveTag1 = "maskblue";
    std::string preserveTag2 = "maskyellow";
    std::string preserveTag3 = "maskred";

    std::vector<PlayerState> playerStates {1};
    float colisionOffset = 1.5f;
    bool  debug = false;

    Config() {
        edCamera.position = (Vector3){ 25, 25.0f, 50 };
        edCamera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        edCamera.up = (Vector3){ 0.0f, 1.f, 0.0f };
        edCamera.fovy = 25.0f;
        edCamera.projection = CAMERA_ORTHOGRAPHIC;
    }

    void onSerialize(ISerialize* inSerialize, World &world)
    {
        inSerialize->propertyFloat("camera_fov",edCamera.fovy);
        inSerialize->propertyBool("grid",grid);
        inSerialize->propertyFloat("animationTime",animationTime);
        bool value = edCamera.projection == CAMERA_ORTHOGRAPHIC;
        inSerialize->propertyBool("camera_ortographic",value);
        edCamera.projection = value ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE;
        inSerialize->propertyString("startingLevel",startingLevel);
        inSerialize->propertyString("gameoverText",gameoverText);
        inSerialize->propertyString("gameoverRetryText",gameoverRetryText);
        inSerialize->propertyString("preserveTag1",preserveTag1);
        inSerialize->propertyString("preserveTag2",preserveTag2);
        inSerialize->propertyString("preserveTag3",preserveTag3);
        inSerialize->propertyFloat("colisionOffset",colisionOffset);
        inSerialize->propertyBool("debug",debug);

        int num = playerStates.size();
        inSerialize->propertyInt("statesNum",num);
        if (num<1) num = 1;
        if ( playerStates.size() != num)
            playerStates.resize(num);

        playerStates[0].requiredTag = "";

        inSerialize->propertyStruct("states",[&](ISerialize* inSerialize) {
            for(int i = 0; i < num ; i++)
            {
                inSerialize->propertyStruct(std::to_string(i),[&](ISerialize* inSerialize) {
                    playerStates[i].onSerialize(inSerialize,world);
                });
            }
        });
    }
};
