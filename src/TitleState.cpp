#include "TitleState.h"
#include "DiscoState.h"
#include "Game.h"
#include "HallwayState.h"
#include "LabState.h"
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
    spriter->Open("recursos/img/menu.png");
    spriter->SetCameraFollower(true);
    titleGO->AddComponent(spriter);
    titleGO->box.x = -100;
    AddObject(titleGO);

    // Configurações comuns de texto
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    std::string fontPath = "recursos/font/heavy heap.otf";
    int fontSize = 36;
    int startY = 550; // Posição Y inicial para o primeiro botão
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
    SDL_Color continueColor = (faseSalva.empty() || faseSalva == "Jogo_Concluido") ? SDL_Color{128, 128, 128, 100} : white;

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
    Text *txtNewGame = new Text(*btnNewGame, fontPath, fontSize, BLENDED, "NOVO JOGO", white);
    btnNewGame->AddComponent(txtNewGame);
    txtNewGame->SetCameraFollower(true);
    btnNewGame->box.x = posX + 35;
    btnNewGame->box.y = startY + spacing;
    AddObject(btnNewGame);

    // Botão: Mudar Música (Mute) ------------------------------------------------------------------
    btnMute = new GameObject();
    std::string muteStr = GameData::isMuted ? "MUTAR: LIGADO" : "MUTAR: DESLIGADO";
    muteTextPtr = new Text(*btnMute, fontPath, fontSize, BLENDED, muteStr, black);
    btnMute->AddComponent(muteTextPtr);
    muteTextPtr->SetCameraFollower(true);
    btnMute->box.x = 30;
    btnMute->box.y = 30;
    AddObject(btnMute);

    // Botão: Sair ---------------------------------------------------------------------------------
    btnExit = new GameObject();
    Text *txtExit = new Text(*btnExit, fontPath, fontSize, BLENDED, "SAIR", black);
    btnExit->AddComponent(txtExit);
    txtExit->SetCameraFollower(true);
    btnExit->box.x = 30;
    btnExit->box.y = 30 + (spacing);
    AddObject(btnExit);

    // Carregar música -----------------------------------------------------------------------------
    backgroundMusic.Open("recursos/audio/Menu.ogg");
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
    if (input.KeyPress(SDLK_9)) {
        GameData::expert = true;
        GameData::easy = false;
    }
    if (input.KeyPress(SDLK_8)) {
        GameData::expert = false;
        GameData::easy = false;
    }
    if ((input.KeyPress(SDLK_7))) {
        GameData::expert = false;
        GameData::easy = true;
    }

    if ((input.KeyPress(SDLK_1))) {
        GameData::target_stage = 1;
    }
    if ((input.KeyPress(SDLK_2))) {
        GameData::target_stage = 2;
    }
    if ((input.KeyPress(SDLK_3))) {
        GameData::target_stage = 3;
    }

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
                    Load_Hallway();
                    return;
                }
                else if (faseSalva == "Fase_3")
                {
                    Load_Pub();
                    return;
                }
                else
                {
                    Load_Lab();
                    return;
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
            if (GameData::target_stage == 2) {
                Load_Hallway();
            }
            else if (GameData::target_stage == 3) {
                Load_Pub();
            }
            else {
                Load_Lab();
            }
            
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

void TitleState::Load_Lab()
{
    std::vector<ResourceItem> labResources = {
        {"recursos/font/heavy heap.otf", TYPE_FONT, 24},
        {"recursos/img/wigly.png", TYPE_IMAGE},
        {"recursos/img/lab_bg.png", TYPE_IMAGE},
        {"recursos/img/lab.png", TYPE_IMAGE}};

    Game::GetInstance().Push(new LoadingState(new LabState(), labResources, false));
}

void TitleState::Load_Hallway()
{
    std::vector<ResourceItem> hallwayResources = {
        {"recursos/font/heavy heap.otf", TYPE_FONT, 24},
        {"recursos/img/wigly.png", TYPE_IMAGE},
        {"recursos/img/purplehaze.png", TYPE_IMAGE},
        {"recursos/img/Bouncer_SpriteSheet.png", TYPE_IMAGE},
        {"recursos/img/potions.png", TYPE_IMAGE},
        {"recursos/audio/Fundo.mp3", TYPE_MUSIC}};

    Game::GetInstance().Push(new LoadingState(new HallwayState(), hallwayResources, false));
    return;
}

void TitleState::Load_Pub()
{
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
        {"recursos/audio/playlist/BW.mp3", TYPE_MUSIC},
        {"recursos/audio/Erro.mp3", TYPE_SOUND},
        {"recursos/audio/Right_Place.mp3", TYPE_SOUND}};

    // Cria o DiscoState, mas joga ele envelopado
    DiscoState *proximaFase = new DiscoState();
    Game::GetInstance().Push(new LoadingState(proximaFase, discoResources, false));
    return;
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