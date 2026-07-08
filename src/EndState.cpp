#include "EndState.h"
#include "SpriteRenderer.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Game.h"
#include "GameData.h"
#include "HallwayState.h"
#include "DiscoState.h"
#include "LoadingState.h"
#include <iostream>
#include <fstream>

EndState::EndState() {}

EndState::~EndState() = default;

void EndState::LoadAssets()
{
    // 1. Fundo do Estado -------------------------------------------------------------------------
    auto *bgObject = new GameObject();
    auto *bgRenderer = new SpriteRenderer(*bgObject);

    SDL_Color white = {255, 255, 255, 255}; // Branco

    if (GameData::playerVictory_3)
    {
        bgRenderer->Open("recursos/img/Win.png");
        backgroundMusic.Open("recursos/audio/endStateWin.ogg");
    }
    else
    {
        bgRenderer->Open("recursos/img/GAME_OVER.jpg");
        backgroundMusic.Open("recursos/audio/teen_titans.mp3");
    }

    bgRenderer->SetCameraFollower(true);
    bgObject->box.x = 0;
    bgObject->box.y = 0;
    bgObject->AddComponent(bgRenderer);
    AddObject(bgObject);

    backgroundMusic.Play();

    // Botões-----------------------------------------
    std::string fontPath = "recursos/font/neodgm.ttf";
    int fontSize = 24;
    int posY = 620;

    if (!GameData::playerVictory_3) // layout de derrot
    {
        // Botão: Tentar Novamente
        btnRetry = new GameObject();
        btnRetry->layer = 10.0;
        Text *txtRetry = new Text(*btnRetry, fontPath, fontSize, BLENDED, "TENTAR NOVAMENTE", white);
        btnRetry->AddComponent(txtRetry);
        txtRetry->SetCameraFollower(true);
        btnRetry->box.x = 250;
        btnRetry->box.y = posY;
        AddObject(btnRetry);

        // Botão: Voltar ao Menu
        btnMenu = new GameObject();
        btnMenu->layer = 10.0;
        Text *txtMenu = new Text(*btnMenu, fontPath, fontSize, BLENDED, "VOLTAR AO MENU", white);
        btnMenu->AddComponent(txtMenu);
        txtMenu->SetCameraFollower(true);
        btnMenu->box.x = 540;
        btnMenu->box.y = posY;
        AddObject(btnMenu);

        // Botão: Sair do Jogo
        btnExit = new GameObject();
        btnExit->layer = 10.0;
        Text *txtExit = new Text(*btnExit, fontPath, fontSize, BLENDED, "SAIR DO JOGO", white);
        btnExit->AddComponent(txtExit);
        txtExit->SetCameraFollower(true);
        btnExit->box.x = 850;
        btnExit->box.y = posY;
        AddObject(btnExit);

        // Texto grande de Game Over
        GameObject *gameOverTextGO = new GameObject();
        gameOverTextGO->layer = 15.0;
        gameOverTextGO->box.x = 120;
        gameOverTextGO->box.y = 100;

        Text *txtGameOver = new Text(*gameOverTextGO, "recursos/font/neodgm.ttf", 72, BLENDED, "GAME OVER", white);
        gameOverTextGO->AddComponent(txtGameOver);
        txtGameOver->SetCameraFollower(true);

        AddObject(gameOverTextGO);
    }
    else
    {

        // Botão: Voltar ao Menu 
        btnMenu = new GameObject();
        btnMenu->layer = 10.0;
        Text *txtMenu = new Text(*btnMenu, fontPath, fontSize, BLENDED, "VOLTAR AO MENU", white);
        btnMenu->AddComponent(txtMenu);
        txtMenu->SetCameraFollower(true);
        btnMenu->box.x = 400;
        btnMenu->box.y = posY;
        AddObject(btnMenu);

        // Botão: Sair do Jogo
        btnExit = new GameObject();
        btnExit->layer = 10.0;
        Text *txtExit = new Text(*btnExit, fontPath, fontSize, BLENDED, "SAIR DO JOGO", white);
        btnExit->AddComponent(txtExit);
        txtExit->SetCameraFollower(true);
        btnExit->box.x = 720;
        btnExit->box.y = posY;
        AddObject(btnExit);

        // Retry nulo 
        btnRetry = nullptr;
    }

    // Estrutura do Autosave -------------------------------------------------------------------
    std::cout << "Salvando o progresso...\n";
    std::ofstream saveFile("save.txt");
    if (saveFile.is_open())
    {
        if (GameData::playerVictory_3)
            saveFile << "Fase_Atual=Jogo_Concluido\n";
        else if (GameData::playerVictory_2)
            saveFile << "Fase_Atual=Fase_3\n";
        else if (GameData::playerVictory_1)
            saveFile << "Fase_Atual=Fase_2\n";
        else
            saveFile << "Fase_Atual=Fase_1\n";

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

    /*
    if (input.QuitRequested())
    {
        quitRequested = true;
        return;
    }
    */

    if (input.MousePress(LEFT_MOUSE_BUTTON))
    {
        // TENTAR NOVAMENTE
        if (IsButtonClicked(btnRetry))
        {
            popRequested = true; // Remove o EndState

            // Verifica qual fase recarregar
            if (GameData::currentState == 3)
            {
                // Recursos do DiscoState
                std::vector<ResourceItem> discoResources = {
                    {"recursos/font/neodgm.ttf", TYPE_FONT, 24},
                    {"recursos/img/Piso-pub-disco.png", TYPE_IMAGE},
                    {"recursos/img/Dancefloor.png", TYPE_IMAGE},
                    {"recursos/img/wigly.png", TYPE_IMAGE},
                    {"recursos/img/discoGhost.png", TYPE_IMAGE},
                    {"recursos/img/Holofote.png", TYPE_IMAGE},
                    {"recursos/img/potions.png", TYPE_IMAGE},
                    {"recursos/audio/Fundo.mp3", TYPE_MUSIC},
                    {"recursos/audio/vinilswipe.mp3", TYPE_SOUND},
                    {"recursos/audio/playlist/IWS.mp3", TYPE_MUSIC},
                    {"recursos/audio/playlist/BW.mp3", TYPE_MUSIC},
                    {"recursos/audio/Erro.mp3", TYPE_SOUND},
                    {"recursos/audio/Right_Place.mp3", TYPE_SOUND}};

                Game::GetInstance().Push(new LoadingState(new DiscoState(), discoResources, false));
            }
            else
            {
                // Recursos do HallwayState
                std::vector<ResourceItem> hallwayResources = {
                    {"recursos/font/neodgm.ttf", TYPE_FONT, 24},
                    {"recursos/img/wigly.png", TYPE_IMAGE},
                    {"recursos/img/purplehaze.png", TYPE_IMAGE},
                    {"recursos/img/Bouncer_SpriteSheet.png", TYPE_IMAGE},
                    {"recursos/img/potions.png", TYPE_IMAGE},
                    {"recursos/audio/Fundo.mp3", TYPE_MUSIC}};

                Game::GetInstance().Push(new LoadingState(new HallwayState(), hallwayResources, false));
            }
            return;
        }

        // VOLTAR AO MENU
        if (IsButtonClicked(btnMenu))
        {
            popRequested = true;
            return;
        }

        // SAIR DO JOGO
        if (IsButtonClicked(btnExit))
        {
            quitRequested = true;
            return;
        }
    }
}

bool EndState::IsButtonClicked(GameObject *buttonGO)
{
    if (!buttonGO)
        return false;

    InputManager &input = InputManager::GetInstance();
    int mouseX = input.GetMouseX();
    int mouseY = input.GetMouseY();

    return (mouseX >= buttonGO->box.x && mouseX <= buttonGO->box.x + buttonGO->box.w &&
            mouseY >= buttonGO->box.y && mouseY <= buttonGO->box.y + buttonGO->box.h);
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

void EndState::Pause() {}
void EndState::Resume() {}