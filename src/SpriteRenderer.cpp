#include "../include/SpriteRenderer.h"
#include "../include/Camera.h"

// Construtor sem arquivo de imagem
SpriteRenderer::SpriteRenderer(GameObject &associated) : Component(associated), sprite() {}

// Construtor que já carrega um sprite
SpriteRenderer::SpriteRenderer(GameObject &associated, const std::string &file, int frameCountW, int frameCountH)
    : Component(associated), sprite(file, frameCountW, frameCountH)
{

    // Define o tamanho da box do GameObject baseado no sprite carregado
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();

    // Inicia no primeiro frame
    sprite.SetFrame(0);
}

// Carrega um novo sprite e ajusta a box do GameObject
void SpriteRenderer::Open(const std::string &file)
{
    sprite.Open(file);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
}

// Quantidade de frames
void SpriteRenderer::SetFrameCount(int frameCountW, int frameCountH)
{
    sprite.SetFrameCount(frameCountW, frameCountH);
}

// Qual frame será exibido
void SpriteRenderer::SetFrame(int frame, SDL_RendererFlip frameFlip)
{
    sprite.SetFrame(frame);
    sprite.SetFlip(frameFlip);
}

// Atualiza o componente (deixamos vazio por enquanto)
void SpriteRenderer::Update(float dt) {}

// Renderiza o sprite na posição do GameObject
void SpriteRenderer::Render()
{
    sprite.Render(associated.box.x, associated.box.y, associated.angleDeg);
}

// Controlar se o sprite deve ignorar o deslocamento da câmera
void SpriteRenderer::SetCameraFollower(bool follow)
{
    sprite.SetCameraFollower(follow);
}

// Identifica o tipo
bool SpriteRenderer::Is(const std::string &type)
{
    return type == "SpriteRenderer";
}

//DEBUG
void SpriteRenderer::Start()
{
    // Nada por enquanto
}

// Trabalho 5
void SpriteRenderer::SetScale(float scaleX, float scaleY)
{
    Vec2 oldCenter = associated.box.GetCenter();
    sprite.SetScale(scaleX, scaleY);
    associated.box.w = sprite.GetWidth();
    associated.box.h = sprite.GetHeight();
    associated.box.SetCenter(oldCenter); // mantém o centro
}
