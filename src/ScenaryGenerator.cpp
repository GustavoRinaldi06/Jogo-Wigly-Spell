#include "ScenaryGenerator.h"
#include "GameObject.h"
#include "State.h" 
#include "Game.h"
#include "GameData.h"
#include "Floor.h"
#include "Background.h"
#include "HallwayState.h"

#include <iostream>

const int ScenaryGenerator::pattern[6][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,1,1,0,0,0,0,1,0},{0,1,0,0,0,0,1,1,0,0},{1,1,0,0,0,0,0,0,1,1},{0,0,1,0,0,0,0,1,0,0},{0,1,0,1,0,0,1,0,1,0}};
const int ranges[5] = {6,1,2,1,1};
const float followrates[5] = {1,0.8,0.5,0.2};
const float widths[5] = {120,1380,1380,1380,1380};

ScenaryGenerator::ScenaryGenerator(GameObject &associated, Vec2 spd, Vec2 start, Vec2 end,SDL_RendererFlip flp, float rot, int typ)
    : Component(associated)
{
    type = typ;
    speed = spd;
    speed.x = spd.x;
    speed.y = spd.y;
    basestart = start;
    baseend = end;
    flip = flp;
    angle = rot;
    if (type ==0) {
        range = 6;
        followrate = 1;
        batchcount = 10;
    }
    else if (type == 2)  {
        range = 2;
        batchcount = 1;
    }
    else {
        range = 1;
        batchcount = 1;
    }
    GenerateBatch(Vec2(start.x,start.y),0);
    if (abs(GameData::universalspeed.x) > 0) {
        GenerateBatch(Vec2(reference->box.x+reference->box.w,start.y),1);
    }
    
    int pat =  rand() % range;
    while (range > 1 && pat == lastpattern) {
        pat = rand() % range;
    }
    if (abs(GameData::universalspeed.x) > 0) {
        GenerateBatch(Vec2(reference->box.x+reference->box.w,start.y),pat);
    }
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

    for (int i = 0; i < batchcount; i++)
    {
        GameObject* go = new GameObject();
        
        Game::GetInstance().GetCurrentState().AddObject(go);
        if (type == 0) {
            Floor *tile = new Floor(*go,"recursos/img/floor.png",ScenaryGenerator::pattern[pattern][i],flip,angle);
            go->AddComponent(tile);
            tile->speed = speed;
            tile->limit = baseend.x;
            
        }
        else {
            Background *BG;
            if (type == 1 ) {
                BG = new Background(*go,"recursos/img/CorredorL1.png",0,flip,2,followrates[type]);
            }
            else if (type == 2 && pattern == 0) {
                BG = new Background(*go,"recursos/img/CorredorL2.png",0,flip,1,followrates[type]);
            }
            else if (type == 2 && pattern == 1) {
                BG = new Background(*go,"recursos/img/Corredor2L2.png",0,flip,1,followrates[type]);
            }
            else if (type == 3) {
                BG = new Background(*go,"recursos/img/CorredorL3.png",0,flip,0,followrates[type]);
            }

            else if (type == 4) {
                BG = new Background(*go,"recursos/img/Pub layer 1.png",0,flip,10,followrates[type]);
            }
            else if (type == 5) {
                BG = new Background(*go,"recursos/img/Pub layer 2.png",0,flip,2,followrates[type]);
            }
            else if (type == 6) {
                
                BG = new Background(*go,"recursos/img/Pub layer 3.png",0,flip,1,followrates[type]);
            }
            else if (type == 7) {
                BG = new Background(*go,"recursos/img/Pub layer 4.jpg",0,flip,0,followrates[type]);
            }
            go->AddComponent(BG);
            BG->speed = speed;
            BG->limit = -baseend.x;;
        }
        go->box.x = Start.x+(go->box.w)*i;
        go->box.y = Start.y;    
        reference = go;
    }

    genTimer.Restart();
}


void ScenaryGenerator::Update(float dt)
{
    genTimer.Update(dt);
    Vec2 uspeed = GameData::universalspeed;
    float batchTime = (widths[type]*batchcount) / abs(speed.x + followrates[type]*uspeed.x);
    if (genTimer.Get() >=  batchTime) {
        int pat = rand() % range;
        while (range > 1 && pat == lastpattern) {
            pat = rand() % range;
        }
        float refx = 0;
        float refw = 0; 
        if  (reference != nullptr) {
            refx = reference->box.x;
            refw = reference->box.w;
        }
        if (abs(GameData::universalspeed.x) > 0) {
            GenerateBatch(Vec2(refx+refw,basestart.y),pat);
        }
    }
}

bool ScenaryGenerator::Is(const std::string &type)
{
    return type == "ScenaryGenerator";
}