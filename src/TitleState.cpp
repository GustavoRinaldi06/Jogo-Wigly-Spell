#include "TitleState.h"
#include "Game.h"
#include "StageState.h"
#include "InputManager.h"
#include "SpriteRenderer.h"
#include "Text.h"
#include "GameObject.h"

#define INCLUDE_SDL
#include "SDL_include.h"

TitleState::TitleState()
    : State()
{}

TitleState::~TitleState() = default;

void TitleState::LoadAssets()
{
// Carregar imagem ------------------------------------------------------------------------------------
    GameObject *titleGO = new GameObject();

    SpriteRenderer *spriter = new SpriteRenderer(*titleGO);
    spriter->Open("recursos/img/Abertura.png"); 
    spriter->SetCameraFollower(true); // igual a imagem de fundo   

    titleGO->AddComponent(spriter);
    AddObject(titleGO);

// Carregar texto ------------------------------------------------------------------------------------
    SDL_Color black = {0, 0, 0, 255};
    GameObject *textGO = new GameObject();
    pressStart = new Text(*textGO, "recursos/font/neodgm.ttf", 24, BLENDED, "PRESS SPACE TO START !", black);
    textGO->AddComponent(pressStart);

    pressStart->SetCameraFollower(true);
    // Posição do texto
    textGO->box.x = 480;
    textGO->box.y = 600;

    AddObject(textGO);
    
// Carregar musica -----------------------------------------------------------------------------------
    backgroundMusic.Open("recursos/audio/Abertura.mp3");
    backgroundMusic.Play();
}

void TitleState::Update(float dt)
{
    // VErificar ações de input ---------------------------------------

    InputManager &input = InputManager::GetInstance();

    // Ao clicar X na janela
    if (input.QuitRequested() || input.KeyPress(SDLK_ESCAPE))
    {
        quitRequested = true;
        return;
    }

    // Se ESPAÇO for pressionado, empilha StageState
    if (input.KeyPress(SDLK_SPACE))
    {
        Game::GetInstance().Push(new StageState());
        return;
    }

    // FAzer o texto piscar ---------------------------------------

    if (!pressStart)
        return;

    static float timerBlink = 0.0f;
    static bool visible = true;
    timerBlink += Game::GetInstance().GetDeltaTime();
    if (timerBlink >= 0.5f)
    {
        // alterna os valores e zera o contador de blink
        visible = !visible;
        timerBlink = 0.0f;
        pressStart->SetText(visible ? "PRESS SPACE TO START" : ""); //Tira texto para piscar
    }
}

void TitleState::Render()
{
    SDL_Renderer *renderer = Game::GetInstance().GetRenderer();

    // Limpa
    SDL_RenderClear(renderer);

    // Desenha
    RenderArray();

    // Apresenta
    SDL_RenderPresent(renderer);
}

void TitleState::Start()
{
    LoadAssets();
    StartArray();
    started = true;
}
