#ifndef GAME_H
#define GAME_H

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include <string>
#include <stack>  
#include <memory>

class State;

class Game
{
public:
    // Destruidor
    ~Game();
    // Inicializador do jogo
    void Run();

    SDL_Renderer *GetRenderer(); // Retorna o renderizador SDL

    // Controle de State
    void Push(State *state); // Empilha um estado
    State &GetCurrentState(); // Retorna estado no topo da pilha

    static Game &GetInstance();   
    float GetDeltaTime() const;

private:
    // Construtor
    Game(const std::string &title, int width, int height);

    // Atributos
    static Game *instance;  // Instância única da classe
    SDL_Window *window;     // Janela do jogo
    SDL_Renderer *renderer; // Renderer para desenhar no jogo

    // State
    std::stack<std::unique_ptr<State>> stateStack; // pilha de estados
    State *storedState;                            // estado atual

    // Auxiliam no gerenciamento
    void InitSDL(); // Inicializar SDL, imagem e mixer
    void QuitSDL(); // Finalizar SDL, imagem e mixer

    // Temporizador
    int frameStart;
    float dt;
    void CalculateDeltaTime();   
};
#endif 
