// SDL
#ifdef INCLUDE_SDL
#ifdef _WIN32
#include <SDL2/SDL.h>
#elif __linux__
#include <SDL2/SDL.h>
#else
#error "Unknown compiler"
#endif
#undef INCLUDE_SDL
#endif // INCLUDE_SDL

// Imagem
#ifdef INCLUDE_SDL_IMAGE
#ifdef _WIN32
#include <SDL2/SDL_image.h>
#elif __linux__
#include <SDL2/SDL_image.h>
#else
#error "Unknown compiler"
#endif
#undef INCLUDE_SDL_IMAGE
#endif // INCLUDE_SDL_IMAGE

// Mixer
#ifdef INCLUDE_SDL_MIXER
#ifdef _WIN32
#include <SDL2/SDL_mixer.h>
#elif __linux__
#include <SDL2/SDL_mixer.h>
#else
#error "Unknown compiler"
#endif
#undef INCLUDE_SDL_MIXER
#endif // INCLUDE_SDL_MIXER

// TTF
#ifdef INCLUDE_SDL_TTF
#ifdef _WIN32
#include <SDL2/SDL_ttf.h>
#elif __linux__
#include <SDL2/SDL_ttf.h>
#else
#error "Unknown compiler"
#endif
#undef INCLUDE_SDL_TTF
#endif // INCLUDE_SDL_TTF
