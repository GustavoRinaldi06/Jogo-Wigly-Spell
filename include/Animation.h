#ifndef ANIMATION_H
#define ANIMATION_H

#define INCLUDE_SDL
#include "SDL_include.h"

class Animation
{
public:
// Guarda as informações de uma animação
    Animation(int frameStart, int frameEnd, float frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int frameStart;
    int frameEnd;
    float frameTime;

    SDL_RendererFlip flip;
};

#endif // ANIMATION_H
