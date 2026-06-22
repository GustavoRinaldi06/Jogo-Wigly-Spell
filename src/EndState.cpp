#include "EndState.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "GameObject.h"

EndState::EndState() {}

EndState::~EndState() = default;

void EndState::LoadAssets()
{
    auto *bgObject = new GameObject();

    auto *bgRenderer = new SpriteRenderer(*bgObject);

    // Musica e imagem de derrrota depende de qual fase está
    if (GameData::currentState == 1){
        if (GameData::playerVictory_1) // Se o jogador ganhou
        {
            bgRenderer->Open("recursos/img/Win.png");
            backgroundMusic.Open("recursos/audio/endStateWin.ogg");
        }
        else // Se o jogador morreu
        {
            bgRenderer->Open("recursos/img/Fim.png");
            backgroundMusic.Open("recursos/audio/teen_titans.mp3");
        }
    }

    if (GameData::currentState == 2)
    {
        if (GameData::playerVictory_2) // Se o jogador ganhou
        {
            bgRenderer->Open("recursos/img/Win.png");
            backgroundMusic.Open("recursos/audio/endStateWin.ogg");
        }
        else // Se o jogador morreu
        {
            bgRenderer->Open("recursos/img/Fim.png");
            backgroundMusic.Open("recursos/audio/teen_titans.mp3");
        }
    }

    if (GameData::currentState == 3)
    {
        if (GameData::playerVictory_3) // Se o jogador ganhou
        {
            bgRenderer->Open("recursos/img/Win.png");
            backgroundMusic.Open("recursos/audio/endStateWin.ogg");
        }
        else // Se o jogador morreu
        {
            bgRenderer->Open("recursos/img/Fim.png");
            backgroundMusic.Open("recursos/audio/teen_titans.mp3");
        }
    }

    bgRenderer->SetCameraFollower(true);

    bgObject->box.x = 0;
    bgObject->box.y = 0;

    bgObject->AddComponent(bgRenderer);
    AddObject(bgObject);

    backgroundMusic.Play(); // musica de vitória ou derrota
}

void EndState::Update(float dt)
{
    InputManager &input = InputManager::GetInstance();

    // Se apertar ESC, sai do jogo
    if (input.QuitRequested() || input.KeyPress(SDLK_ESCAPE))
    {
        quitRequested = true;
        return;
    }

    // Se apertar ESPAÇO, volta ao TitleState
    if (input.KeyPress(SDLK_SPACE))
    {
        popRequested = true;
        return;
    }
}

void EndState::Render()
{
    RenderArray();
}

void EndState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}

void EndState::Pause()
{}

void EndState::Resume()
{}
