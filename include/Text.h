#ifndef TEXT_H
#define TEXT_H

#include "Component.h"
#include <string>

#define INCLUDE_SDL_TTF
#include "SDL_include.h"


// Define os estilos de renderização de texto
enum TextStyle
{
    SOLID,
    SHADED,
    BLENDED
};

class Text : public Component
{
public:
    Text(GameObject &associated, const std::string &arqv, int tamanho, TextStyle estilo, const std::string &texto, SDL_Color cor);
    ~Text();

    void Update(float dt) override;
    void Render() override;

    // Sets para definir as características do texto
    void SetText(const std::string &text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontSize(int fontSize);

    bool Is(const std::string &type) override;
    void SetCameraFollower(bool follow);

private:
    std::string arqFonte; // Caminho para o arquivo
    int tamFonte;         // Tamanho 
    TextStyle estilo;      // (SOLID, SHADED, BLENDED)
    SDL_Color cor;     
    std::string texto;     
    SDL_Texture *texture; // Textura gerada pelo TTF
    SDL_Rect clipRect;    // Define a região que será desenhada

    void RemakeTexture(); // destroi textura e refaz ela
    bool cameraFollower = false;
};

#endif
