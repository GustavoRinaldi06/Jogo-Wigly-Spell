#include "../include/Game.h"
#include "../include/HallwayState.h"
#include "../include/TitleState.h"
#include "../include/EndState.h"

#define INCLUDE_SDL
#include "SDL_include.h"

int main(int argc, char **argv)
{
    // Cria o estado inicial
    Game &game = Game::GetInstance();
    game.Push(new TitleState()); // adiciona estado de titulo, primeiro da pilha
    game.Run();
    return 0;
}
