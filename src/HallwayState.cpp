#include "../include/HallwayState.h"
#include "../include/TileSet.h"
#include "../include/SpriteRenderer.h"
#include "../include/TileMap.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/Character.h"
#include "../include/BounceBall.h"
#include "../include/HomingProj.h"
#include "../include/Missile.h"
#include "../include/CorrGhost.h"
#include "../include/Potion.h"
#include "../include/Floor.h"
#include "../include/Haze.h"
#include "../include/PlayerController.h"
#include "../include/Collider.h"
#include "../include/Collision.h"
#include "../include/ScenaryGenerator.h"

#include "Text.h"

#define INCLUDE_SDL
#include "SDL_include.h"

#include <iostream>
#include <algorithm>

HallwayState::HallwayState() {}

HallwayState::~HallwayState()
{
    objectArray.clear(); // Limpa os GameObjects automaticamente graças ao unique_ptr
}

void HallwayState::LoadAssets()
{
    std::cout << "\n Carregando fase 2 WiglySpell:" << "\n"; // Alertar LoadAssets
    GameData::universalspeed = Vec2(-50, 0);
    // Fundo -------------------------------------------------------------------------------------------------------------------
    // GameObject *bgObject = new GameObject();
    // SpriteRenderer *bgRenderer = new SpriteRenderer(*bgObject);
    // bgRenderer->Open("recursos/img/FundoTest.png");
    // bgRenderer->SetCameraFollower(false);

    // bgObject->box.x = 0;
    // bgObject->box.y = 0;

    // bgObject->AddComponent(bgRenderer);
    // AddObject(bgObject);

    // Mapa --------------------------------------------------------------------------------------------------------------------

    GameObject *floorGO = new GameObject();
    floorGO->AddComponent(new ScenaryGenerator(*floorGO, Vec2(0.0, 0.0), Vec2(0, 650), Vec2(-120, 0), SDL_FLIP_NONE, 0.0)); // substitua pela imagem correta
    AddObject(floorGO);

    GameObject *ceilGO = new GameObject();
    ceilGO->AddComponent(new ScenaryGenerator(*ceilGO, Vec2(0.0, 0.0), Vec2(0, -64), Vec2(-120, 0), SDL_FLIP_VERTICAL, 0.0)); // substitua pela imagem correta
    AddObject(ceilGO);

    GameObject *bgGO = new GameObject();
    bgGO->AddComponent(new ScenaryGenerator(*bgGO, Vec2(0.0, 0.0), Vec2(0, 0), Vec2(-1380, 0), SDL_FLIP_NONE, 0.0, 1)); // substitua pela imagem correta
    AddObject(bgGO);
    bgGO = new GameObject();
    bgGO->AddComponent(new ScenaryGenerator(*bgGO, Vec2(0.0, 0.0), Vec2(0, 0), Vec2(-1380, 0), SDL_FLIP_NONE, 0.0, 2)); // substitua pela imagem correta
    AddObject(bgGO);

    bgGO = new GameObject();
    bgGO->AddComponent(new ScenaryGenerator(*bgGO, Vec2(0.0, 0.0), Vec2(0, 0), Vec2(-1380, 0), SDL_FLIP_NONE, 0.0, 3)); // substitua pela imagem correta
    AddObject(bgGO);

    // Personagem ----------------------------------------------------------------------------------------------------------------
    GameObject *playerGO = new GameObject();
    playerGO->box.x = 500; // Centro do mapa
    playerGO->box.y = 300; // Altura maior

    playerGO->AddComponent(new Character(*playerGO, "recursos/img/wigly.png")); // substitua pela imagem correta
    playerGO->AddComponent(new PlayerController(*playerGO));

    GameData::playerHP = 100;                      // Reseta vida do personagem
    Camera::GetInstance().SetPosition(Vec2(0, 0)); // Puxa a câmera de volta pro (0,0)

    // Camera::GetInstance().Follow(playerGO); // Segue o novo personagem

    AddObject(playerGO);
    GameData::inverted = false;
    float prevy = 0;
    for (int i = 0; i < 8; i++)
    {
        GameObject *hazeGO = new GameObject();
        hazeGO->AddComponent(new Haze(*hazeGO, "recursos/img/purplehaze.png", SDL_FLIP_NONE, 0)); // substitua pela imagem correta
        hazeGO->box.x = 0;                                                                        // Centro do mapa
        hazeGO->box.y = prevy;                                                                    // Altura maior
        AddObject(hazeGO);
        prevy = hazeGO->box.y + hazeGO->box.h;
    }

    GameObject *corridorGO = new GameObject();
    corridorGO->box.x = 750; // Centro do mapa
    corridorGO->box.y = 200; // Altura maior

    bossComponent = new CorridorGhost(*corridorGO, "recursos/img/Bouncer_SpriteSheet.png");
    corridorGO->AddComponent(bossComponent);
    AddObject(corridorGO);

    // Música --------------------------------------------------------------------------------------------------------------------
    backgroundMusic.Open("recursos/audio/Fundo.mp3");
    if (!GameData::isMuted)
    {
        backgroundMusic.Play(-1);
    }

    // Texto da vida do personagem------------------------------------------------------------------------------------------------

    SDL_Color white = {255, 255, 255, 255};
    GameObject *textGO = new GameObject();
    std::string hpString = "HP: " + std::to_string(Character::player->GetHP());
    hpText = new Text(*textGO, "recursos/font/neodgm.ttf", 24, BLENDED, hpString, white);
    textGO->AddComponent(hpText);
    textGO->layer = 10;

    hpText->SetCameraFollower(true);
    // Posição do texto
    textGO->box.x = 60;
    textGO->box.y = 650;

    AddObject(textGO);

    // TExto de cooldown da magia --------------------------------------------------------------------------------------------
    /*
    GameObject *textGO1 = new GameObject();
    std::string spellCooldown = "Magia acumulada com sucesso!";
    spellText = new Text(*textGO1, "recursos/font/neodgm.ttf", 24, BLENDED, spellCooldown, white);
    textGO1->AddComponent(spellText);
    textGO1->layer = 10;

    spellText->SetCameraFollower(true);
    // Posição do texto
    textGO1->box.x = 200;
    textGO1->box.y = 650;

    AddObject(textGO1);

    */

    GameObject *cauldGO1 = new GameObject();
    cauldGO1->AddComponent(new Potion(*cauldGO1, "recursos/img/potions.png", 0));
    cauldGO1->layer = 10;

    // Posição do texto
    cauldGO1->box.x = 200;
    cauldGO1->box.y = 650;

    AddObject(cauldGO1);

    GameObject *cauldGO2 = new GameObject();
    cauldGO2->AddComponent(new Potion(*cauldGO2, "recursos/img/potions.png", 1));
    cauldGO2->layer = 10;

    // Posição do texto
    cauldGO2->box.x = 250;
    cauldGO2->box.y = 650;
    AddObject(cauldGO2);

    GameObject *textGO2 = new GameObject();
    std::string bhpString = "Boss HP: " + std::to_string(GameData::bossHP);
    bosshpText = new Text(*textGO2, "recursos/font/neodgm.ttf", 24, BLENDED, bhpString, white);
    textGO2->AddComponent(bosshpText);
    textGO2->layer = 10;

    bosshpText->SetCameraFollower(true);
    // Posição do texto
    textGO2->box.x = 700;
    textGO2->box.y = 650;

    AddObject(textGO2);
}

void HallwayState::Update(float dt)
{
    InputManager &input = InputManager::GetInstance();

    Camera::GetInstance().Update(dt);

    // Se o jogador pressionou ESC ou clicou no X ou esc
    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY))
    {
        quitRequested = true;
        return;
    }

    // DEBUG DE TESTE  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (input.KeyPress(SDLK_m))
    {
        std::cout << "Troca de modos acionada!" << std::endl;
        if (GameData::gameMode == 0)
            GameData::gameMode = 1; // trocca para plataforma
        else
            GameData::gameMode = 0; // troca para menu
    }

    // Leitura da tecla I
    if (input.KeyPress(SDLK_i))
    {
        std::cout << "Gravidade invertida!" << std::endl;
        if (GameData::inverted)
            GameData::inverted = false;
        else
            GameData::inverted = true;
    }

    // Leitura da tecla K
    if (input.KeyPress(SDLK_k))
    {
        std::cout << "Modo de mira alterado!" << std::endl;
        if (GameData::aimed)
            GameData::aimed = false;
        else
            GameData::aimed = true;
    }

    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Atualiza todos os GameObjects
    UpdateArray(dt);

    // Atualiza colisores
    for (auto &obj : objectArray)
    {
        Collider *collider = (Collider *)obj->GetComponent("Collider");
        if (collider)
        {
            collider->Update(0); // forçando update
        }
    }

    // Verifica colisões dos GameObjects
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

                    // Se algum morreu, não faz a outra colisão
                    if (!objA->IsDead() && !objB->IsDead())
                    {
                        objB->NotifyCollision(*objA);
                    }
                }
            }
        }
    }

    // Remove os mortos
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

    // CAixa de texto de vida
    if (hpText && Character::player != nullptr)
    {
        std::string hpString = "HP: " + std::to_string(Character::player->GetHP());
        hpText->SetText(hpString);
    }
    if (bosshpText)
    {
        std::string bhpString = "";

        if (GameData::bossHP <= 0)
        {
            bhpString = "Boss HP: 0";
        }
        else
        {
            bhpString = "Boss HP: " + std::to_string(GameData::bossHP);
        }

        bosshpText->SetText(bhpString);
    }

    // Caixa do cooldown do disparo
    if (spellText && Character::player != nullptr)
    {
        if (GameData::spell == true)
        {
            if (Character::player->GetCool() < 5)
            {
                std::string spellCooldown = "Acumulando magia novamente... " + std::to_string(5 - Character::player->GetCool());
                spellText->SetText(spellCooldown);
            }
            else
            {
                std::string spellCooldown = "Magia acumulada com sucesso!";
                spellText->SetText(spellCooldown);
            }
        }
        else
            spellText->SetText("");
    }

    // Checagem de vitoria
    if (!bossDefeated && (bossComponent == nullptr || bossComponent->health <= 0))
    {
        bossDefeated = true;
        victoryTimer.Restart(); // Zera o cronômetro para começar a contar
        GameData::playerVictory_2 = true;
    }

    // Se o Boss já foi derrotado
    if (bossDefeated)
    {
        victoryTimer.Update(dt);

        float delay = 3.0f; // Tempo em segundos (ex: 2 segundos de delay)
        if (victoryTimer.Get() >= delay)
        {
            popRequested = true;
            Game::GetInstance().Push(new DiscoState());
            return;
        }
    }

    // Checagem de fim Derrota
    if (Character::player == nullptr || Character::player->GetGameObject()->IsDead()) // Se o player tiver morrido
    {
        GameData::playerVictory_2 = false;
        popRequested = true;
        Game::GetInstance().Push(new EndState());
        return;
    }
}

void HallwayState::Render()
{
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();

    // Limpa tudo para o próximo frame
    SDL_RenderClear(renderer);

    // Desenha todos os game objects
    std::sort(objectArray.begin(), objectArray.end(), [](const std::shared_ptr<GameObject> a, const std::shared_ptr<GameObject> b)
              { return a->layer < b->layer; });
    RenderArray();

    // Apresenta o frame
    SDL_RenderPresent(renderer);
}

void HallwayState::Start()
{
    LoadAssets();
    StartArray(); // Agora a classe state é responsável
    started = true;
    GameData::currentState = 2; // Essa sendo a seg fase
    GameData::gameMode = 1;
    GameData::playerVictory_1 = false; // Reinicia a fase e apaga save de vitória anterior
}

void HallwayState::Pause()
{
    // Não sei ainda
}

void HallwayState::Resume()
{
    // Não sei ainda
}