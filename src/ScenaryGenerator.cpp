#include "ScenaryGenerator.h"
#include "GameObject.h"
#include "State.h" 
#include "Game.h"
#include "GameData.h"
#include "Floor.h"
#include "StageState.h"

#include <iostream>

const int ScenaryGenerator::pattern[6][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,1,1,0,0,0,0,1,0},{0,1,0,0,0,0,1,1,0,0},{1,1,0,0,0,0,0,0,1,1},{0,0,1,0,0,0,0,1,0,0},{0,1,0,1,0,0,1,0,1,0}};

ScenaryGenerator::ScenaryGenerator(GameObject &associated, Vec2 spd, Vec2 start, Vec2 end,SDL_RendererFlip flp, float rot)
    : Component(associated)
{
    speed = spd;
    speed.x = spd.x;
    speed.y = spd.y;
    basestart = start;
    baseend = end;
    flip = flp;
    angle = rot;
    GenerateBatch(Vec2(0,start.y),0);
    int pat =  rand() % 6;
    while (pat == lastpattern) {
        pat = rand() % 6;
    }
    
    GenerateBatch(Vec2(reference->box.x+reference->box.w,start.y),pat);
    while (pat == lastpattern) {
        pat = rand() % 6;
    }
    GenerateBatch(Vec2(reference->box.x+reference->box.w,start.y),pat);
    
    genTimer.Restart();
}

ScenaryGenerator::~ScenaryGenerator()
{
}

void ScenaryGenerator::Render()
{

}

void ScenaryGenerator::GenerateBatch(Vec2 Start, int pattern) {
    lastpattern = pattern;
    for (int i = 0; i < 10; i++)
    {
        GameObject* go = new GameObject();
        go->box.x = Start.x+120*i;
        go->box.y = Start.y;
        Game::GetInstance().GetCurrentState().AddObject(go);
        Floor *tile = new Floor(*go,"recursos/img/floor.png",ScenaryGenerator::pattern[pattern][i],flip,angle);
        go->AddComponent(tile);
        tile->speed = speed;
        tile->limit = -baseend.x;
        reference = go;
    }
    genTimer.Restart();

    
}


void ScenaryGenerator::Update(float dt)
{
    genTimer.Update(dt);
    Vec2 uspeed = GameData::universalspeed;
    float batchTime = (1200.0) / abs(speed.x + uspeed.x);
    if (genTimer.Get() >=  batchTime) {
        int pat = rand() % 6;
        while (pat == lastpattern) {
            pat = rand() % 6;
        }
        float refx = 0;
        float refw = 0; 
        if  (reference != nullptr) {
            refx = reference->box.x;
            refw = reference->box.w,basestart.y;
        }
        GenerateBatch(Vec2(refx+refw),pat);
    }
}

bool ScenaryGenerator::Is(const std::string &type)
{
    return type == "ScenaryGenerator";
}