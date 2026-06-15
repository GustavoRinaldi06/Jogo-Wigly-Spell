#include "TitleState.h"
#include "Game.h"
#include "HallwayState.h"
#include "InputManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameData.h"

#define INCLUDE_SDL
#include "SDL_include.h"

TitleState::TitleState() : State() {}

TitleState::~TitleState() = default;

void TitleState::LoadAssets()
{
    // 1. Carregar imagem de fundo --------------------------------------------------------------------
    GameObject *titleGO = new GameObject();
    SpriteRenderer *spriter = new SpriteRenderer(*titleGO);
    spriter->Open("recursos/img/Abertura.png");
    spriter->SetCameraFollower(true);
    titleGO->AddComponent(spriter);
    AddObject(titleGO);

    // Configurações comuns de texto
    //SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    std::string fontPath = "recursos/font/neodgm.ttf";
    int fontSize = 24;
    int startY = 400; // Posição Y inicial para o primeiro botão
    int spacing = 60; // Espaçamento vertical entre os botões
    int posX = 480;   // Posição X dos botões

    // Botão: Continuar Jogo -----------------------------------------------------------------------
    btnContinue = new GameObject();
    Text *txtContinue = new Text(*btnContinue, fontPath, fontSize, BLENDED, "CONTINUAR JOGO", black);
    btnContinue->AddComponent(txtContinue);
    txtContinue->SetCameraFollower(true);
    btnContinue->box.x = posX;
    btnContinue->box.y = startY;
    AddObject(btnContinue);

    // Botão: Novo Jogo ----------------------------------------------------------------------------
    btnNewGame = new GameObject();
    Text *txtNewGame = new Text(*btnNewGame, fontPath, fontSize, BLENDED, "NOVO JOGO", black);
    btnNewGame->AddComponent(txtNewGame);
    txtNewGame->SetCameraFollower(true);
    btnNewGame->box.x = posX;
    btnNewGame->box.y = startY + spacing;
    AddObject(btnNewGame);

    // Botão: Mudar Música (Mute) ------------------------------------------------------------------
    btnMute = new GameObject();
    std::string muteStr = GameData::isMuted ? "MUTAR: LIGADO" : "MUTAR: DESLIGADO";
    muteTextPtr = new Text(*btnMute, fontPath, fontSize, BLENDED, muteStr, black);
    btnMute->AddComponent(muteTextPtr);
    muteTextPtr->SetCameraFollower(true);
    btnMute->box.x = posX;
    btnMute->box.y = startY + (spacing * 2);
    AddObject(btnMute);

    // Botão: Sair ---------------------------------------------------------------------------------
    btnExit = new GameObject();
    Text *txtExit = new Text(*btnExit, fontPath, fontSize, BLENDED, "SAIR", black);
    btnExit->AddComponent(txtExit);
    txtExit->SetCameraFollower(true);
    btnExit->box.x = posX;
    btnExit->box.y = startY + (spacing * 3);
    AddObject(btnExit);

    // Carregar música -----------------------------------------------------------------------------
    backgroundMusic.Open("recursos/audio/Abertura.mp3");
    if (!GameData::isMuted)
    {
        backgroundMusic.Play();
    }
}

void TitleState::Update(float dt)
{
    InputManager &input = InputManager::GetInstance();

    // Fechar se clicar no X da janela
    if (input.QuitRequested())
    {
        quitRequested = true;
        return;
    }

    // Verifica cliques nos botões
    if (input.MousePress(LEFT_MOUSE_BUTTON))
    {
        // Botão 1: Continuar Jogo
        if (IsButtonClicked(btnContinue))
        {
            // Lógica de buscar jogo salvo aqui...
            // Exemplo: GameData::LoadGame();
            // Game::GetInstance().Push(new HallwayState());
            return;
        }

        // Botão 2: Novo Jogo (Empilha HallwayState)
        if (IsButtonClicked(btnNewGame))
        {
            Game::GetInstance().Push(new HallwayState());
            return;
        }

        // Botão 3: Silenciar Música
        if (IsButtonClicked(btnMute))
        {
            GameData::isMuted = !GameData::isMuted; // Inverte o estado global

            if (GameData::isMuted)
            {
                backgroundMusic.Stop();
                muteTextPtr->SetText("MUTAR: LIGADO");
            }
            else
            {
                backgroundMusic.Play();
                muteTextPtr->SetText("MUTAR: DESLIGADO");
            }
            return;
        }

        // Botão 4: Sair do Jogo
        if (IsButtonClicked(btnExit))
        {
            quitRequested = true;
            return;
        }
    }
}

// Função auxiliar para checar se o clique do mouse ocorreu dentro da caixa do botão
bool TitleState::IsButtonClicked(GameObject *buttonGO)
{
    if (!buttonGO)
        return false;

    InputManager &input = InputManager::GetInstance();
    int mouseX = input.GetMouseX();
    int mouseY = input.GetMouseY();

    // Checa colisão Ponto (Mouse) vs Retângulo (Box do GameObject)
    return (mouseX >= buttonGO->box.x && mouseX <= buttonGO->box.x + buttonGO->box.w &&
            mouseY >= buttonGO->box.y && mouseY <= buttonGO->box.y + buttonGO->box.h);
}

void TitleState::Render()
{
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();
    SDL_RenderClear(renderer);
    RenderArray();
    SDL_RenderPresent(renderer);
}

void TitleState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}