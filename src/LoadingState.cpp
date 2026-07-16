#include "../include/LoadingState.h"
#include "../include/Resources.h"
#include "../include/Game.h"
#include "../include/SpriteRenderer.h"
#include <iostream>

LoadingState::LoadingState(State *nextState, std::vector<ResourceItem> resourcesToLoad, bool cleanResources)
    : nextStatePtr(nextState), assetsList(resourcesToLoad), currentAssetIndex(0), clearedPrevious(false), shouldClean(cleanResources)
{
    SDL_Color white = {255, 255, 255, 255};

    // Caldeirão Animado
    caldeiraoGO = new GameObject();
    caldeiraoGO->layer = 9;
    caldeiraoGO->box.x = 580;
    caldeiraoGO->box.y = 280;

    SpriteRenderer *spriteRend = new SpriteRenderer(*caldeiraoGO, "recursos/img/Caldeirao.png", 4, 8);
    spriteRend->SetScale(2.0f, 2.0f);
    spriteRend->SetTransparency(255);
    caldeiraoGO->AddComponent(spriteRend);

    Animator *animator = new Animator(*caldeiraoGO);
    animator->AddAnimation("loading", Animation(0, 3, 0.15f));
    caldeiraoGO->AddComponent(animator);
    animator->SetAnimation("loading");

    objectArray.emplace_back(caldeiraoGO);

    // Texto da Porcentagem
    textGO = new GameObject();
    textGO->layer = 10;
    textGO->box.x = 580;
    textGO->box.y = 600;

    percentageText = new Text(*textGO, "recursos/font/heavy heap.otf", 36, BLENDED, "0%", white);
    textGO->AddComponent(percentageText);
    objectArray.emplace_back(textGO);

    // Texto aleatorio
    guessingTextGO = new GameObject();
    guessingTextGO->layer = 10;
    // guessingTextGO->box.x = 400;
    guessingTextGO->box.y = 440;

    std::vector<std::string> magicPhrases = {
        "Quem e esse...? Eita jogo errado.",
        "Consultando a playlist do grimorio...",
        "Misturando os ingredientes...",
        "Tchau, tchau fantasmas!"};

    int randomIndex = rand() % magicPhrases.size();
    std::string chosenPhrase = magicPhrases[randomIndex];

    guessingText = new Text(*guessingTextGO, "recursos/font/heavy heap.otf", 25, BLENDED, chosenPhrase, white);
    guessingTextGO->AddComponent(guessingText);
    guessingTextGO->box.x = 620.0f - (guessingTextGO->box.w / 2.0f); // para centralizar os textos

    objectArray.emplace_back(guessingTextGO);

    minimumDisplayTimer.Restart(); // para dar uma pausa

    /*
    loadMusic.Open("");
    if (!GameData::isMuted)
    {
        loadMusic.Play(-1);
    }
    */
}

LoadingState::~LoadingState()
{
    objectArray.clear();
}

void LoadingState::LoadAssets() {}

void LoadingState::Start()
{
    started = true;
}

void LoadingState::Update(float dt)
{
    minimumDisplayTimer.Update(dt);

    // Controle se deve ou não limpar
    if (!clearedPrevious)
    {
        if (shouldClean)
        {
            Resources::ClearImages();
            Resources::ClearMusics();
            Resources::ClearSounds();

            SpriteRenderer *spriteRend = (SpriteRenderer *)caldeiraoGO->GetComponent("SpriteRenderer"); // Ajuste o nome se for diferente no seu motor
            if (spriteRend)
            {
                spriteRend->Open("recursos/img/Caldeirao.png");
            }
        }
        clearedPrevious = true;
        return;
    }

    // 2. Carrega um Asset por frame
    if (currentAssetIndex < assetsList.size())
    {
        ResourceItem item = assetsList[currentAssetIndex];

        if (item.type == TYPE_IMAGE)
        {
            Resources::GetImage(item.path);
        }
        else if (item.type == TYPE_MUSIC)
        {
            Resources::GetMusic(item.path);
        }
        else if (item.type == TYPE_SOUND)
        {
            Resources::GetSound(item.path);
        }
        else if (item.type == TYPE_FONT)
        {
            Resources::GetFont(item.path, item.fontSize);
        }

        currentAssetIndex++;

        int percentage = (int)(((float)currentAssetIndex / (float)assetsList.size()) * 100.0f);
        percentageText->SetText(std::to_string(percentage) + "%");
    }
    else if (minimumDisplayTimer.Get() >= 3.5f)
    {
        popRequested = true;
        Game::GetInstance().Push(nextStatePtr.release());
    }

    UpdateArray(dt);
}

void LoadingState::Render()
{
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderArray();

    SDL_RenderPresent(renderer);
}

void LoadingState::Pause() {}
void LoadingState::Resume() {}