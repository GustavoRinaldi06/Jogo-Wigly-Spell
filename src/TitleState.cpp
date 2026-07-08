#include "TitleState.h"
#include "DiscoState.h"
#include "Game.h"
#include "HallwayState.h"
#include "InputManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameData.h"
#include "LoadingState.h"

#include <fstream> // Incluído para leitura do save.txt

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
    SDL_Color black = {0, 0, 0, 255};
    std::string fontPath = "recursos/font/neodgm.ttf";
    int fontSize = 24;
    int startY = 400; // Posição Y inicial para o primeiro botão
    int spacing = 60; // Espaçamento vertical entre os botões
    int posX = 480;   // Posição X dos botões

    // Verificar se existe um save válido para definir a cor do botão Continuar ---------------------
    std::string faseSalva = "";
    std::ifstream saveFile("save.txt");
    if (saveFile.is_open())
    {
        std::string linha;
        if (std::getline(saveFile, linha))
        {
            size_t pos = linha.find('=');
            if (pos != std::string::npos)
            {
                faseSalva = linha.substr(pos + 1);
            }
        }
        saveFile.close();
    }

    // se não houver save ou o jogo já foi concluído, deixa o botão cinza
    SDL_Color continueColor = (faseSalva.empty() || faseSalva == "Jogo_Concluido") ? SDL_Color{128, 128, 128, 100} : black;

    // Botão: Continuar Jogo -----------------------------------------------------------------------
    btnContinue = new GameObject();
    Text *txtContinue = new Text(*btnContinue, fontPath, fontSize, BLENDED, "CONTINUAR JOGO", continueColor);
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
        backgroundMusic.Play(-1);
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
            std::ifstream saveFile("save.txt");
            std::string faseSalva = "";

            if (saveFile.is_open())
            {
                std::string linha;
                if (std::getline(saveFile, linha))
                {
                    size_t pos = linha.find('=');
                    if (pos != std::string::npos)
                        faseSalva = linha.substr(pos + 1);
                }
                saveFile.close();
            }

            // Apenas prossegue se houver um save válido e não finalizado
            if (!faseSalva.empty() && faseSalva != "Jogo_Concluido")
            {
                if (faseSalva == "Fase_2")
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
                    return;
                }
                else if (faseSalva == "Fase_3")
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
                    return;
                }
                else
                {
                    Game::GetInstance().Push(new HallwayState()); // por enquanto não há a primeira fase
                }
                return;
            }

            // Clique ignorado caso não exista arquivo de save
            return;
        }

        // Botão 2: Novo Jogo (Empilha HallwayState)
        if (IsButtonClicked(btnNewGame))
        {
            // Mapeia todos os arquivos do DiscoState
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

            // Cria o DiscoState, mas joga ele envelopado
            DiscoState *proximaFase = new DiscoState();
            Game::GetInstance().Push(new LoadingState(proximaFase, discoResources, false));
            return;
        }

        // Botão 3: Silenciar Música
        if (IsButtonClicked(btnMute))
        {
            GameData::isMuted = !GameData::isMuted;

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

// checar se o clique do mouse ocorreu dentro da caixa do botão
bool TitleState::IsButtonClicked(GameObject *buttonGO)
{
    if (!buttonGO)
        return false;

    InputManager &input = InputManager::GetInstance();
    int mouseX = input.GetMouseX();
    int mouseY = input.GetMouseY();

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