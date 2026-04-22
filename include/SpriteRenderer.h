#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"

class SpriteRenderer : public Component {
public:
    // Construtores
    SpriteRenderer(GameObject &associated);
    SpriteRenderer(GameObject &associated, const std::string &file, int frameCountW = 1, int frameCountH = 1);

    // Abre um novo sprite
    void Open(const std::string &file);

    // Define a quantidade de frames do sprite
    void SetFrameCount(int frameCountW, int frameCountH);

    // Define qual frame deve ser renderizado
    void SetFrame(int frame, SDL_RendererFlip frameFlip);

    // Atualiza o componente (por enquanto vazio)
    void Update(float dt) override;

    // Renderiza o sprite
    void Render() override;

    // Verifica se o componente é um SpriteRenderer
    bool Is(const std::string &type) override;

    // Correção da camera
    void SetCameraFollower(bool follow);

    //DEBUG
    void Start() override;

    // Trabalho 5
    void SetScale(float x, float y);

private:
    Sprite sprite;
};

#endif // SPRITERENDERER_H
