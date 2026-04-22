#include "../include/Music.h"
#include "../include/Resources.h"

// Construtores
Music::Music() : music(nullptr){}

Music::Music(const std::string &file) : music(nullptr)
{
    Open(file);
}

// Destruidor
Music::~Music()
{
    Stop(1500);
    music = nullptr;
}

// abre a música
void Music::Open(const std::string &file)
{
    music = Resources::GetMusic(file);
    if (music == nullptr)
    {
        SDL_Log("Falha ao carregar música: %s\n", Mix_GetError());
    }
}
// Verfica se está aberta
bool Music::IsOpen() const
{
    return music != nullptr;
}

// Controle da musica
void Music::Play(int times)
{
    Mix_PlayMusic(music, times);
}
void Music::Stop(int ms)
{
    Mix_FadeOutMusic(ms);
}