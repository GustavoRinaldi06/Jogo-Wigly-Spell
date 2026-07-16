#include "../include/Game.h"
#include "../include/Resources.h"
#include "../include/InputManager.h"
#include "../include/State.h"
#include "../include/GameData.h"
#include <iostream>
#include <fstream>

// Inicializa a variável estática 'instance' como nullptr
Game *Game::instance = nullptr;

Game &Game::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Game("WIGLY SPELL", 1200, 720);
    }
    return *instance;
}

// Construtor
Game::Game(const std::string &title, int width, int height)
{
    if (instance != nullptr)
    {
        std::cerr << "Já existe uma instância do jogo\n";
        return;
    }

    instance = this;
    InitSDL();  // Inicializa a SDL

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window)
    {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << "\n";
        QuitSDL();
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Erro ao criar renderizador: " << SDL_GetError() << "\n";
        QuitSDL();
        exit(1);
    }
    SDL_RenderSetLogicalSize(renderer,width,height);
    


    

    frameStart = SDL_GetTicks();
    dt = 0;

    storedState = nullptr; // Inicializa como nullptr
}

// Destrutor
Game::~Game()
{
    while (!stateStack.empty())
    {
        stateStack.pop();
    }

    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();

    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    Mix_CloseAudio();
    QuitSDL();
}

// Empilha um novo estado
void Game::Push(State *state)
{
    storedState = state;
}

// Retorna o estado atual (topo da pilha)
State &Game::GetCurrentState()
{
    return *stateStack.top();
}

// Loop do jogo
void Game::Run()
{
    if (storedState != nullptr)
    {
        stateStack.emplace(storedState);
        stateStack.top()->Start();
        storedState = nullptr;
    }
    else if (stateStack.empty())
    {
        return; // Sem estado inicial, encerra
    }

    while (!stateStack.empty() && !stateStack.top()->QuitRequested()) // Não tiver estados ou estado superior solicitar fechar janela
    {
        CalculateDeltaTime();
        InputManager::GetInstance().Update();

        // Checa se o estado do topo pediu para sair
        if (stateStack.top()->PopRequested())
        {
            stateStack.pop();
            if (!stateStack.empty())
            {
                stateStack.top()->Resume();
            }
        }

        // Checa se há novo estado para empilhar
        if (storedState != nullptr)
        {
            if (!stateStack.empty())
            {
                stateStack.top()->Pause();
            }
            stateStack.emplace(storedState);
            stateStack.top()->Start();
            storedState = nullptr;
        }

        // Atualiza e renderiza o estado atual
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        if (!stateStack.empty())
        {
            stateStack.top()->Update(dt);
            stateStack.top()->Render();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(33); // ~30 FPS
    }

    // estrutura do autosave0
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
        if (GameData::Permanent_playerVictory_1){
            saveFile << "Complete1=1\n";
        }
        else {
            saveFile << "Complete1=0\n";
        }
        if (GameData::Permanent_playerVictory_2){
            saveFile << "Complete2=1\n";
        }
        else {
            saveFile << "Complete2=0\n";
        }
        if (GameData::Permanent_playerVictory_3){
            saveFile << "Complete3=1\n";
        }
        else {
            saveFile << "Complete3=0\n";
        }

        saveFile.close();
        std::cout << "Progresso salvo com sucesso em save.txt!\n";
    }
    else
    {
        std::cerr << "Erro ao criar o arquivo de save!\n";
    }
}

// Inicializador de SDL
void Game::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << "\n";
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "Erro ao inicializar SDL_ttf: " << TTF_GetError() << "\n";
        SDL_Quit();
        exit(1);
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    int imgInitResult = IMG_Init(imgFlags);
    if ((imgInitResult & imgFlags) != imgFlags)
    {
        std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError() << "\n";
        SDL_Quit();
        exit(1);
    }

    int mixFlags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG;
    int mixInitResult = Mix_Init(mixFlags);
    if ((mixInitResult & mixFlags) != mixFlags)
    {
        std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << "\n";
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        std::cerr << "Erro ao abrir áudio: " << Mix_GetError() << "\n";
        QuitSDL();
        exit(1);
    }

    Mix_AllocateChannels(32);
}

void Game::QuitSDL()
{
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::CalculateDeltaTime()
{
    int currentFrameStart = SDL_GetTicks();
    dt = (currentFrameStart - frameStart) / 1000.0f;
    if (dt > 0.1) {
        dt = 0.1;
    }
    frameStart = currentFrameStart;
}

SDL_Renderer *Game::GetRenderer()
{
    return renderer;
}

float Game::GetDeltaTime() const
{
    return dt;
}
