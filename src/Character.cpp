#include "Character.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h" 
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "InputManager.h"
#include "GameData.h"
#include "StageState.h"

#include "Floor.h"

#include <iostream>

Character *Character::player = nullptr;

Character::Character(GameObject &associated, const std::string &spritePath)
    : Component(associated), linearSpeed(250), hp(100)
{
    if (player == nullptr){
        player = this;
    }

    auto renderer = new SpriteRenderer(associated, spritePath, 3, 4);
    renderer->SetCameraFollower(false);
    associated.AddComponent(renderer);


    //associated.box.w = 50;  // ou a largura desejada
    //associated.box.h = 100; // altura desejada

    // Novos sons
    //hitSound = Sound();
    //deathSound = Sound();
    //fallSound = Sound();
    walkSound = Sound("recursos/audio/AndandoGrama.mp3");
    spellSound = Sound("recursos/audio/Spell.mp3");
    spell2Sound = Sound("recursos/audio/Spell2.mp3");
    noSpell = Sound("recursos/audio/NoSpell.mp3");

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(6, 9, 0.5f));
    animator->AddAnimation("walking_X", Animation(0, 5, 0.2f));
    animator->AddAnimation("walking_UP", Animation(6, 9, 0.5f));
    animator->AddAnimation("walking_DOWN", Animation(6, 9, 0.5f));
    animator->AddAnimation("dead", Animation(10, 11, 0.3f));
    associated.AddComponent(animator);

    Collider *col = new Collider(associated);


    col->SetScale(Vec2(0.65,1.0));
    //col->SetOffset(Vec2(associated.box.w*(1-0.65)/2,0));

    associated.AddComponent(col);
    spellTimer.Set(0.8);
    spell2Timer.Set(5.0);
 
    wasInverted = GameData::inverted; // Faz track da direção da gravidade
    Inversion = false;
}

Character::~Character()
{
    if (player == this)
        player = nullptr;
}

void Character::Start()
{}


void Character::Update(float dt)
{
    // Ao morrer -------------------------------------------------------------------------------
    if (associated.box.y > 750)
    {
        // dispara animação e som apenas uma vez
        if (!deathAnimTriggered)
        {
            hp = 0;
            GameData::playerHP = hp;
            deathAnimTriggered = true;
            fallSound.Play(1);

            // seta animação "dead"
            Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
            if (animator)
                animator->SetAnimation("dead");

            // solta a câmera se é player
            if(this == Character::player)
                Camera::GetInstance().Unfollow();

            deathTimer.Restart();
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 6s
        if (deathTimer.Get() > 6.0f)
            associated.RequestDelete();

        return; // não executa mais lógica de movimento
    }

    // Enquanto vivo --------------------------------------------------------------------------
    damageCooldown.Update(dt);
    GameData::playerHP = hp; // Atualiza variavel global

    // Pega input de pulo ---------------------------------------------------------------------
    InputManager &input = InputManager::GetInstance();
    if (GameData::gameMode == 1){
        SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
        if (dashing) {
            dashTimer.Update(dt);
            float dtimer = dashTimer.Get();
            if (dtimer < 0.15) {
                if (!input.IsKeyDown(SDLK_LSHIFT)) {
                    longdash = false;   // Se não segura por tempo suficiente, o dash é curto
                }
            }
            if (dtimer > 0.15 && !longdash || dtimer > 0.3 && longdash) {
                dashing = false;
                dashTimer.Restart();
                speed.x = 0;
            }
            else {
                speed.x = facingDir *500.0f;
            }
        }
        if (input.KeyPress(SDLK_LSHIFT) && !dashed)
        {
            speed.x = facingDir * 500.0f;  // ajustar a força de pulo na grvidade invertida
            speed.y = 0;
            dashed = true; // se pulou
            dashing = true;
            dashTimer.Restart();
            longdash = true;
        } 
        if (jumping) {
            jumpTimer.Update(dt);
            float jtimer = jumpTimer.Get();
            if (jtimer < 0.15) {
                if (!input.IsKeyDown(SDLK_SPACE)) {
                    jumping = false;   // Se não segura por tempo suficiente, o salto é curto
                    jumpTimer.Restart();
                    speed.y *= 0.5;
                }
            }
            else {

                jumping = false;
                jumpTimer.Restart();
            }
        }
        if (!dashing && input.KeyPress(SDLK_SPACE) && isOnGround)
        {
            if (GameData::inverted == false)
                speed.y = -700.0f; // ajustar a força de pulo
            else if (GameData::inverted == true)
                speed.y = 700.0f;  // ajustar a força de pulo na grvidade invertida

            // Pulou
            isOnGround = false; // se está no chão
            jumped = true; // se pulou
            jumping = true;
        }
        else if (!dashing && input.KeyPress(SDLK_SPACE) && jumped && (!Djumped) && GameData::Djump == true)
        {
            if (GameData::inverted == false)
                speed.y = -600.0f; // ajustar a força de pulo
            else if (GameData::inverted == true)
                speed.y = 600.0f; // ajustar a força de pulo na grvidade invertida
                
            Djumped = true;      // se pulou duas vezes
            jumping = true;
        }
    }

    // Aplica a gravidade --------------------------------------------------------------------
    if (GameData::gameMode == 1)
    {
        if (GameData::inverted != wasInverted) // Caso houve inversão de gravidade
        {
            isOnGround = false;
            Inversion = true;

            //Reseta os pulos para evitar que o player pule "no ar" logo após inverter
            jumped = false;
            Djumped = false;
            if (!dashing) {
                dashed = false;
            }
            
            // Dá um empurrão na direção da nova gravidade para desgrudar o colisor do chão/teto
            if (GameData::inverted == true){
                speed.y = -15.0f; // Empurra levemente pra cima
            }
            else{
                speed.y = 15.0f; // Empurra levemente pra baixo 
            }

            // Atualiza a memória do estado
            wasInverted = GameData::inverted;
        }

        // Aplica a gravidade --------------------------------------------------------------------
        if (!dashing){
            if (GameData::inverted == false)
                speed.y += gravity * dt;
            else
                speed.y -= gravity * dt; // gravidade invertida, puxa pro teto
        }
    }

    

    // Realiza os movimentos e ações ---------------------------------------------------------
    spellTimer.Update(dt);
    spell2Timer.Update(dt);
    if (!taskQueue.empty())
    {
        Command current = taskQueue.front();

        // MOVE ------------------------------------------------------------------------
        if (!dashing && current.type == CommandType::MOVE)
        {
            Vec2 dir = (current.pos - associated.box.GetCenter()).Normalize();
            speed.x = dir.x * linearSpeed;

            
            if (GameData::gameMode == 0){
                speed.y = dir.y * linearSpeed;
            }
            taskQueue.pop();

        }
        

        
    }
    surfaceTimer.Update(dt);
    if (surfaceTimer.Get() > 0.1) {
        surfacespeed = Vec2(0,0);
    }
    Vec2 uspeed = GameData::universalspeed;
    associated.box.x += (speed.x + surfacespeed.x + uspeed.x)* dt;
    associated.box.y += (speed.y + surfacespeed.y + uspeed.y)* dt;

    // Atualiza animação de acordo com a movimentação
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    if (animator)
    {
        animator->Update(dt);
        if (fabs(speed.x) > 1.0f)
            animator->SetAnimation("walking_X");
        // GameMode menu
        else if (GameData::gameMode == 0)
        { // Decide a direção que vai ficar verticalmente
            if (speed.y < -0.1f)
                animator->SetAnimation("walking_UP");
            else
                animator->SetAnimation("walking_DOWN");
        }
        else animator->SetAnimation("idle");
    }

    // Decide a direção que vai ficar horizontalmente
    if (speed.x < -0.1f)
        facingDir = -1;
    else if (speed.x > 0.1f)
        facingDir = 1;

    // Flip horizontal/vertical de acordo a direção ---------------------------------------------------------------
    SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));

    if (renderer && animator)
    {
        int frame = animator->GetCurrentFrame();

        // Começamos sem nenhum flip
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Se estiver virado para a esquerda, aplica o flip horizontal
        if (facingDir == -1){
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        }

        // Se a gravidade estiver invertida, adiciona o flip vertical ao estado atual (apenas no plataformer)
        if (GameData::inverted && GameData::gameMode == 1){
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        }

        // Aplica o resultado final (que pode ser nenhum, um, ou ambos os flips)
        renderer->SetFrame(frame, flip);

    }

    walkSoundCall(dt);

    speed.x = 0;
    if(GameData::gameMode == 0){
        speed.y = 0;
    }
}

void Character::Render() {}

bool Character::Is(const std::string &type)
{
    return type == "Character";
}

void Character::Issue(Command task)
{
    while (!taskQueue.empty())
        taskQueue.pop();
    taskQueue.push(task);
}

void Character::NotifyCollision(GameObject &other)
{

    // Se colidir com chão
    Collider *collider = (Collider *)other.GetComponent("Collider");
    Collider * col = (Collider *)associated.GetComponent("Collider");
    int dir = col->ColDir(collider);
    if (collider && collider->tag == "solid")
    {
        surfaceTimer.Restart();
        Floor *floor = (Floor *)other.GetComponent("Floor");
        surfacespeed = floor->speed;
        // Ajusta posição
        if (dir == 0) {
            associated.box.y = other.box.y - associated.box.h;
            speed.y = 0;
            if (!GameData::inverted) {
                isOnGround = true; // está tocando no chão
                if (!dashing) {
                    dashed = false;
                }
                if(jumped || Inversion){ // pulou e precisa limpar lista de comandos realizados no ar
                    jumped = false;
                    Djumped = false;
                    Inversion = false;
                    
                    for (int i = 0; i < (int)taskQueue.size(); i++){
                        taskQueue.pop();
                    }
                }
            }
            
        }
        else if (dir == 1) {
            associated.box.y = other.box.y + other.box.h;
            speed.y = 0;
            if (GameData::inverted) {
                isOnGround = true; // está tocando no chão
                if (!dashing) {
                    dashed = false;
                }
                if (jumped || Inversion){ // pulou e precisa limpar lista de comandos realizados no ar
                    jumped = false;
                    Djumped = false;
                    Inversion = false;
                    for (int i = 0; i < (int)taskQueue.size(); i++){
                        taskQueue.pop();
                    }
                }
            }
            
            
        }
        else if (dir == 2) {
            associated.box.x = other.box.x - associated.box.w;
            speed.x = 0;
        }
        else if (dir == 3) {
            associated.box.x = other.box.x + other.box.w;
            speed.x = 0;
        }
    }
}

int Character::GetHP() const
{
    return hp;
}

int Character::GetCool() const
{
    return static_cast<int>(spell2Timer.Get());
}

void Character::walkSoundCall(float dt){
    // Fazer as passadas do wigly e seu som ao andar  (Pataformer)
    if (GameData::gameMode == 1){
        if (fabs(speed.x) > 1.0f && isOnGround == true) // está andando
        {
            walkSoundTimer.Update(dt);

            if (walkSoundTimer.Get() >= 0.3f) // intervalo entre passos
            {
                walkSound.Play(1);
                walkSoundTimer.Restart();
            }
        }
        else
        {
            walkSoundTimer.Restart(); // parar de andar
        }
    }

    // FAzer as passadas do wigly e seu som de andar (Menu)
    else if (GameData::gameMode == 0)
    {
        if (fabs(speed.x) > 1.0f || fabs(speed.y) > 1.0f) // está andando
        {
            walkSoundTimer.Update(dt);

            if (walkSoundTimer.Get() >= 0.3f) // intervalo entre passos
            {
                walkSound.Play(1);
                walkSoundTimer.Restart();
            }
        }
        else
        {
            walkSoundTimer.Restart(); // parar de andar
        }
    }
}

void Character::Shoot1(Vec2 targetPos)
{
    if (spellTimer.Get() >= 0.8)
    {
        spellSound.Play(1);

        Vec2 shooterCenter = associated.box.GetCenter();
        Vec2 delta = targetPos - shooterCenter;

        if (delta.Magnitude() < 0.01f)
        {
            delta = Vec2(1, 0);
        }
        Vec2 direction = delta.Normalize();

        float angle = atan2(direction.y, direction.x);
        float speed = 350.0f;
        int damage = 10;
        float maxDistance = 700.0f;
        bool targetsPlayer = false;

        GameObject *spellGO = new GameObject();
        spellGO->box.x = shooterCenter.x;
        spellGO->box.y = shooterCenter.y - 20;
        spellGO->AddComponent(new Bullet(*spellGO, angle, speed, damage, maxDistance, targetsPlayer, "recursos/img/Bullet.png"));

        Game::GetInstance().GetCurrentState().AddObject(spellGO);

        spellTimer.Restart();
    }
}

void Character::Shoot2(Vec2 targetPos)
{
    if (GameData::spell == true)
    {
        if (spell2Timer.Get() >= 5.0)
        {
            spell2Sound.Play(1);

            Vec2 shooterCenter = associated.box.GetCenter();
            Vec2 delta = targetPos - shooterCenter;

            if (delta.Magnitude() < 0.01f)
            {
                delta = Vec2(1, 0);
            }
            Vec2 direction = delta.Normalize();

            float angle = atan2(direction.y, direction.x);
            float speed = 200.0f;
            int damage = 50;
            float maxDistance = 500.0f;
            bool targetsPlayer = false;

            GameObject *spell2GO = new GameObject();
            spell2GO->box.x = shooterCenter.x;
            spell2GO->box.y = shooterCenter.y - 20;
            spell2GO->AddComponent(new Bullet(*spell2GO, angle, speed, damage, maxDistance, targetsPlayer, "recursos/img/Bullet.png"));

            Game::GetInstance().GetCurrentState().AddObject(spell2GO);

            spell2Timer.Restart();
        }
        else
        {
            noSpell.Play(1); // Tempo de recarga
        }
    }
    else
    {
        noSpell.Play(1); // Não desbloqueou
    }
}

Rect Character::PlayerBox() {
    return player->associated.box;
}