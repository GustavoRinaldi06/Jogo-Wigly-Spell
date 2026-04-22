#ifndef MUSIC_H
#define MUSIC_H

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include <string>

class Music
{
public:
    // Construtores
    Music();
    Music(const std::string &file);

    // Destruidor
    ~Music();

    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(const std::string &file);
    bool IsOpen() const;

private:
    // Ponteiro para a música
    Mix_Music *music;
};

#endif // MUSIC_H
