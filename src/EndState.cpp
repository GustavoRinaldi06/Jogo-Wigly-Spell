#include "EndState.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>

EndState::EndState() {}

EndState::~EndState() = default;

void EndState::LoadAssets()
{
    auto *bgObject = new GameObject();

    auto *bgRenderer = new SpriteRenderer(*bgObject);

    // Musica e imagem de derrrota depende de qual fase está
    if (GameData::playerVictory_3) // Se o jogador ganhou
    {
        bgRenderer->Open("recursos/img/Win.png");
        backgroundMusic.Open("recursos/audio/endStateWin.ogg");
    }
    else // Se o jogador morreu
    {
        bgRenderer->Open("recursos/img/GAME_OVER.jpg");
        backgroundMusic.Open("recursos/audio/teen_titans.mp3");
    }

    bgRenderer->SetCameraFollower(true);

    bgObject->box.x = 0;
    bgObject->box.y = 0;

    bgObject->AddComponent(bgRenderer);
    AddObject(bgObject);

    backgroundMusic.Play(); // musica de vitória ou derrota

    // estrutura do autosave
    std::cout << "Salvando o progresso...\n";
    std::ofstream saveFile("save.txt"); // Cria ou sobrescreve o arquivo save.txt
    if (saveFile.is_open())
    {
        if (GameData::playerVictory_3)
        {
            saveFile << "Fase_Atual=Jogo_Concluido\n";
        }
        else if (GameData::playerVictory_2)
        {
            saveFile << "Fase_Atual=Fase_3\n";
        }
        else if (GameData::playerVictory_1)
        {
            saveFile << "Fase_Atual=Fase_2\n";
        }
        else
        {
            saveFile << "Fase_Atual=Fase_1\n";
        }

        saveFile.close();
        std::cout << "Progresso salvo com sucesso em save.txt!\n";
    }
    else
    {
        std::cerr << "Erro ao criar o arquivo de save!\n";
    }
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
