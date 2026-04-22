#ifndef SOUND_H
#define SOUND_H

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include <string>

class Sound
{
public:
    // Construtores e destruidor
    Sound();
    Sound(const std::string &file);
    ~Sound();

    // Tocar e parar o som
    void Play(int times = 1);
    void Stop();

    // Abrir o arquivo de som
    void Open(const std::string &file);

    // Função para verificar se o som foi aberto
    bool IsOpen() const;

private:
    Mix_Chunk *chunk; // Ponteiro para o chunk de som
    int channel;      // Canal de reprodução
};

#endif // SOUND_H
