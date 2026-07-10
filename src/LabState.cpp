#include "../include/LabState.h"
#include "../include/SpriteRenderer.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Character.h"
#include "../include/DiscoGhost.h"
#include "../include/Potion.h"
#include "../include/Health.h"
#include "../include/PlayerController.h"
#include "../include/Collider.h"
#include "../include/Collision.h"
#include "../include/ScenaryGenerator.h"
#include "../include/Floor.h"
#include "../include/Dancefloor.h"
#include "../include/Resources.h"
#include "../include/Light.h"
#include "../include/HallwayState.h"
#include "../include/LoadingState.h"

#include "Text.h"

#define INCLUDE_SDL
#include "SDL_include.h"

#include <iostream>
#include <algorithm>
#include <vector>

LabState::LabState() {
    currentTutorialFrame = 0;
    tutorialRenderer = nullptr;
    tutorialGameObject = nullptr;
}

LabState::~LabState()
{
    objectArray.clear(); // Limpa os GameObjects automaticamente graças ao unique_ptr
}

void LabState::LoadAssets()
{
    std::cout << "\n Carregando fase 1 WiglySpell (Lab):" << "\n"; // Alertar LoadAssets
    GameData::universalspeed = Vec2(0, 0);

    // Fundo Principal -----------------------------------------------------------------------------------------
    GameObject *bgObject = new GameObject();
    SpriteRenderer *bgRenderer = new SpriteRenderer(*bgObject);
    bgRenderer->Open("recursos/img/lab_bg.png");
    bgRenderer->SetCameraFollower(true);
    bgObject->box.x = 0;
    bgObject->box.y = 0;
    bgObject->layer = 0;
    bgObject->AddComponent(bgRenderer);
    AddObject(bgObject);

    // Camada de Cenário Secundária --------------------------------------------------------------------------------
    GameObject *propsObject = new GameObject();
    SpriteRenderer *propsRenderer = new SpriteRenderer(*propsObject);
    propsRenderer->Open("recursos/img/lab.png");
    propsRenderer->SetCameraFollower(true);
    propsObject->box.x = 0;
    propsObject->box.y = -70;
    propsObject->layer = 10;
    propsObject->AddComponent(propsRenderer);
    AddObject(propsObject);

    GameData::discostart = false;

    // Mapa --------------------------------------------------------------------------------------------
    auto flip = SDL_FLIP_NONE;
    float ypos = 650;
    for (int j = 0; j < 2; j++)
    {
        float prevx = -1536;
        for (int i = 0; i < 14; i++)
        {
            GameObject *floorGO = new GameObject();
            int tipo = 3;
            int frame = 0;
            int id = -1;

            floorGO->AddComponent(new Floor(*floorGO, "recursos/img/floor.png", frame, flip, 0, tipo, id));
            floorGO->box.x = prevx;
            floorGO->box.y = ypos;
            AddObject(floorGO);
            prevx = floorGO->box.x + floorGO->box.w;
        }
        flip = SDL_FLIP_VERTICAL;
        ypos = -64;
    }

    // Personagem ----------------------------------------------------------------------------------------------------------------
    GameObject *playerGO = new GameObject();
    playerGO->box.x = -1450; // Centro do mapa
    playerGO->box.y = 300;   // Altura maior

    playerGO->AddComponent(new Character(*playerGO, "recursos/img/wigly.png")); // substitua pela imagem correta
    playerGO->AddComponent(new PlayerController(*playerGO));

    GameData::playerHP = 100;                          // Reseta vida do personagem
    Camera::GetInstance().SetPosition(Vec2(-1450, 0)); // Puxa a câmera de volta pro (0,0)

    AddObject(playerGO);
    GameData::inverted = false;

    // Música --------------------------------------------------------------------------------------------------------------------
    backgroundMusic.Open("recursos/audio/pubsound.mp3");
    backgroundMusic.Play(-1);

    // skip e tutorial
    SkipSound = Sound("recursos/audio/skip.mp3");
    tutorialSound = Sound("recursos/audio/click.wav");

    //Tutorial
    tutorialGameObject = new GameObject();
    tutorialRenderer = new SpriteRenderer(*tutorialGameObject, "recursos/img/Tutorial.png", 3, 6);
    tutorialRenderer->SetCameraFollower(true);

    tutorialGameObject->box.x = 150;
    tutorialGameObject->box.y = 50;
    tutorialGameObject->layer = 50;

    tutorialGameObject->AddComponent(tutorialRenderer);
    AddObject(tutorialGameObject);
}

void LabState::Update(float dt)
{
    InputManager &input = InputManager::GetInstance();

    Camera::GetInstance().Update(dt);

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY))
    {
        quitRequested = true;
        return;
    }

    if (input.KeyPress(SDLK_t) && !tutorialEnd)
    {
        tutorialEnd = true;
        endTimer.Restart();
        GameData::playerVictory_1 = true;
        SkipSound.Play();

        if (tutorialGameObject != nullptr)
        {
            tutorialGameObject->RequestDelete();
            tutorialGameObject = nullptr;
        }
        tutorialRenderer = nullptr;
    }

    if (input.MousePress(SDL_BUTTON_RIGHT) && tutorialRenderer != nullptr && !tutorialEnd)
    {
        currentTutorialFrame++;
        tutorialSound.Play();

        if (currentTutorialFrame > 16)
        {
            // O tutorial acabou, apagamos a imagem através da referência segura
            if (tutorialGameObject != nullptr)
            {
                tutorialGameObject->RequestDelete();
                tutorialGameObject = nullptr;
            }
            tutorialRenderer = nullptr;

            if (!tutorialEnd)
            {
                tutorialEnd = true;
                endTimer.Restart();
                GameData::playerVictory_1 = true;
                SkipSound.Play();
            }
        }
        else
        {
            tutorialRenderer->SetFrame(currentTutorialFrame);
        }
    }

    if (OverTriggered)
    {
        if (transitionEffect)
        {
            transitionEffect->Update(dt);

            if (transitionEffect->IsFinished())
            {
                popRequested = true;
                Game::GetInstance().Push(new EndState());
                return;
            }
        }
        RenderArray();
        return;
    }

    UpdateArray(dt);

    for (auto &obj : objectArray)
    {
        Collider *collider = (Collider *)obj->GetComponent("Collider");
        if (collider)
        {
            collider->Update(0);
        }
    }

    for (size_t i = 0; i < objectArray.size(); ++i)
    {
        GameObject *objA = objectArray[i].get();
        Component *colCompA = objA->GetComponent("Collider");
        if (!colCompA || objA->IsDead())
            continue;

        Collider *colA = static_cast<Collider *>(colCompA);

        for (size_t j = i + 1; j < objectArray.size(); ++j)
        {
            GameObject *objB = objectArray[j].get();
            Component *colCompB = objB->GetComponent("Collider");
            if (!colCompB || objB->IsDead())
                continue;

            Collider *colB = static_cast<Collider *>(colCompB);

            if (IsColliding(colA->box, objA->angleDeg * M_PI / 180, colB->box, objB->angleDeg * M_PI / 180))
            {
                if (!objA->IsDead() && !objB->IsDead())
                {
                    objA->NotifyCollision(*objB);

                    if (!objA->IsDead() && !objB->IsDead())
                    {
                        objB->NotifyCollision(*objA);
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < objectArray.size();)
    {
        if (objectArray[i]->IsDead())
        {
            objectArray.erase(objectArray.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    if (tutorialEnd)
    {
        endTimer.Update(dt);

        float delay = 1.5f;
        if (endTimer.Get() >= delay && transitionEffect && !transitionEffect->IsFinished())
        {
            transitionEffect->StartOutro();
        }

        if (transitionEffect && transitionEffect->IsFinished())
        {
            popRequested = true;

            Camera::GetInstance().Unfollow();
            Camera::GetInstance().SetPosition(Vec2(0, 0));
            
            std::vector<ResourceItem> hallwayResources = {
                {"recursos/font/heavy heap.otf", TYPE_FONT, 24},
                {"recursos/img/wigly.png", TYPE_IMAGE},
                {"recursos/img/purplehaze.png", TYPE_IMAGE},
                {"recursos/img/Bouncer_SpriteSheet.png", TYPE_IMAGE},
                {"recursos/img/potions.png", TYPE_IMAGE},
                {"recursos/audio/Fundo.mp3", TYPE_MUSIC}};

            Game::GetInstance().Push(new LoadingState(new HallwayState(), hallwayResources, true));
            return;
        }
    }

    if (Character::player == nullptr)
    {
        GameData::playerVictory_1 = false;
        OverTriggered = true;
        if (transitionEffect)
        {
            transitionEffect->StartOutro();
        }
        return;
    }
}

void LabState::Render()
{
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();

    SDL_RenderClear(renderer);

    std::sort(objectArray.begin(), objectArray.end(), [](const std::shared_ptr<GameObject> a, const std::shared_ptr<GameObject> b)
              { return a->layer < b->layer; });
    RenderArray();

    SDL_RenderPresent(renderer);
}

void LabState::Start()
{
    LoadAssets();

    GameObject *transitionGO = new GameObject();
    transitionGO->layer = 99;
    transitionEffect = new Transition(*transitionGO);
    transitionGO->AddComponent(transitionEffect);
    AddObject(transitionGO);

    StartArray();
    started = true;
    GameData::currentState = 1;
    GameData::gameMode = 1;
    GameData::playerVictory_1 = false;
}

void LabState::Pause()
{
}

void LabState::Resume()
{
}