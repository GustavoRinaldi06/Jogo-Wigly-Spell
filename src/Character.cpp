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
#include "HallwayState.h"

#include "Floor.h"

#include <iostream>

Character *Character::player = nullptr;

Character::Character(GameObject &associated, const std::string &spritePath)
    : Component(associated), linearSpeed(250), hp(100)
{
    associated.layer = 5.0;
    if (player == nullptr){
        player = this;
    }

    auto renderer = new SpriteRenderer(associated, spritePath, 4, 12);
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
    spell_red_Sound = Sound("recursos/audio/Spell2.mp3");
    spell_scarlet_Sound = Sound("recursos/audio/Spell2.mp3");
    spell_blue_Sound = Sound("recursos/audio/Spell2.mp3");
    spell_darkBlue_Sound = Sound("recursos/audio/Spell2.mp3");
    spell_purple_Sound = Sound("recursos/audio/Spell2.mp3");
    noSpell = Sound("recursos/audio/NoSpell.mp3");

    // Cria as animações
    auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(22, 25, 0.5f));
    animator->AddAnimation("walking_X", Animation(26, 29, 0.2f));
    animator->AddAnimation("dashstart", Animation(0, 2+1, 0.15f));
    animator->AddAnimation("dashend", Animation(4, 5+1, 0.15f));     //+1 serve para detecção quando a animação deveria trocar
    animator->AddAnimation("dashing", Animation(3, 3, 10.0f));
    animator->AddAnimation("jumping", Animation(16, 17, 0.25f));
    animator->AddAnimation("idleair", Animation(18, 19, 0.25f));
    animator->AddAnimation("walking_wall", Animation(44, 47, 0.25f));
    animator->AddAnimation("idlewall", Animation(32, 43, 0.1));
    animator->AddAnimation("dead", Animation(6, 13, 0.8f));
    associated.AddComponent(animator);

    Collider *col = new Collider(associated); 


    col->SetScale(Vec2(0.65,1.0));
    //col->SetOffset(Vec2(associated.box.w*(1-0.65)/2,0));

    associated.AddComponent(col);
    spellTimer.Set(0.8);
    spellMixTimer.Set(5.0);
 
    wasInverted = GameData::inverted; // Faz track da direção da gravidade
    Inversion = false;

    
    invTimer.Set(100);
    purpleTimer.Set(100);

    GameObject *bubbleGO = new GameObject();
    bubbleGO->AddComponent(new Bubble(*bubbleGO,"recursos/img/bubble.png"));
    Game::GetInstance().GetCurrentState().AddObject(bubbleGO);
    bubble = bubbleGO;

    SpriteRenderer *bubrenderer = (SpriteRenderer *)bubble->GetComponent("SpriteRenderer");
    bubrenderer->SetFrame(0,SDL_FLIP_NONE);

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
    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    
    // Ao morrer -------------------------------------------------------------------------------
    if (associated.box.y > 750 || associated.box.y < -50 || hp <= 0)
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


            
            // solta a câmera se é player e esta com a camera seguindo
            if(this == Character::player)
                Camera::GetInstance().Unfollow();

            deathTimer.Restart();
        }

        SpriteRenderer *renderer = static_cast<SpriteRenderer *>(associated.GetComponent("SpriteRenderer"));
        if (renderer && animator)
        {
            int frame = animator->GetCurrentFrame();

            // sem nenhum flip
            SDL_RendererFlip flip = SDL_FLIP_NONE;

            // Se estiver virado para a esquerda, aplica o flip horizontal
            if (facingDir == -1)
            {
                flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
            }
            // Se a gravidade estiver invertida, adiciona o flip vertical ao estado atual
            if (GameData::inverted && GameData::gameMode == 1)
            {
                flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
            }
            // Aplica o resultado final 
            renderer->SetFrame(frame, flip);
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 6s
        if (deathTimer.Get() > 6.0f){
            bubble->RequestDelete();
            associated.RequestDelete();
        }

        return; // não executa mais lógica de movimento
    }

    // Enquanto vivo --------------------------------------------------------------------------
    damageCooldown.Update(dt);
    invTimer.Update(dt);
    purpleTimer.Update(dt);
    shieldTimer.Update(dt);
    if (damageCooldown.Get() < 2.0) {
        if ((int)((damageCooldown.Get() * 10)) % 4 == 0) {
            transparent = !transparent;
        }
        else {
            transparent = false;
        }
    }
    if (shieldTimer.Get() > 10) {
        shield = 0;
    }
    SpriteRenderer *rend = (SpriteRenderer *)associated.GetComponent("SpriteRenderer");
    if (transparent) {
        rend->SetTransparency(128);
    }
    else {
        rend->SetTransparency(255);
    }
    GameData::playerHP = hp; // Atualiza variavel global

    SpriteRenderer *bubrenderer = (SpriteRenderer *)bubble->GetComponent("SpriteRenderer");
    if (invTimer.Get() < 9) {
        bubrenderer->SetFrame(2,SDL_FLIP_NONE);
    }
    else if (shield > 0) {
        bubrenderer->SetFrame(1,SDL_FLIP_NONE);
    }
    else {
        bubrenderer->SetFrame(0,SDL_FLIP_NONE);
    }
    

    // Pega input de pulo ---------------------------------------------------------------------
    InputManager &input = InputManager::GetInstance();
    if (GameData::gameMode == 1){
        
        if (dashing) {
            dashTimer.Update(dt);
            float dtimer = dashTimer.Get();
            if (dtimer < 0.15) {
                if (!input.IsKeyDown(SDLK_LSHIFT)) {
                    longdash = false;   // Se não segura por tempo suficiente, o dash é curto
                }
            }
            if ((dtimer > 0.15 && !longdash) || (dtimer > 0.3 && longdash)) {
                animator->SetAnimation("dashend");
                dashing = false;
                dashTimer.Restart();
                speed.x = 0;
            }
            else {
                speed.x = facingDir *500.0f;
                if (animator->GetCurrentFrame() == 3 && animator->GetAnimation() == "dashstart") {
                    animator->SetAnimation("dashing");
                }
            }
        }
        if (input.KeyPress(SDLK_LSHIFT) && !dashed)
        {
            speed.x = facingDir * 500.0f;  // ajustar a força de pulo na grvidade invertida
            speed.y = 0;
            dashed = true; // se pulou
            dashing = true;
            animator->SetAnimation("dashstart");
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
            animator->SetAnimation("jumping");
        }
        else if (!dashing && input.KeyPress(SDLK_SPACE) && jumped && (!Djumped) && GameData::Djump == true)
        {
            if (GameData::inverted == false)
                speed.y = -600.0f; // ajustar a força de pulo
            else if (GameData::inverted == true)
                speed.y = 600.0f; // ajustar a força de pulo na grvidade invertida
                
            Djumped = true;      // se pulou duas vezes
            jumping = true;
            animator->SetAnimation("jumping");
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
    spellMixTimer.Update(dt);
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
    
    if (animator)
    {
        animator->Update(dt);
        //  Reseta animações
        if (animator->GetAnimation() == "dashend" && animator->GetCurrentFrame() == 6) {
            if (isOnGround) {
                animator->SetAnimation("idle");
            }
            else {
                animator->SetAnimation("idleair");
            }
        }
        else if (animator->GetAnimation() == "jumping") {
            if (!jumping) {
                if (GameData::inverted && speed.y < 50) {
                    animator->SetAnimation("idleair");
                }
                else if (!GameData::inverted && speed.y > -50) {
                    animator->SetAnimation("idleair");
                }
            }
            
        }
        else if (!dashing) {
            if (fabs(speed.x) > 1.0f && isOnGround)
                animator->SetAnimation("walking_X");
            // GameMode menu
            else if (GameData::gameMode == 0)
            { // Decide a direção que vai ficar verticalmente
                if (speed.Magnitude() > 5.0)
                    animator->SetAnimation("walking_wall");
                else
                    animator->SetAnimation("idlewall");
            }
            else if (isOnGround) {
                animator->SetAnimation("idle");
            }
            else {
                animator->SetAnimation("idleair");
            }
        }
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
    Vec2 campos = (Camera::GetInstance()).GetPosition();
    if (associated.box.x < campos.x) {
        associated.box.x = campos.x;
    }
    if (associated.box.x + associated.box.w > campos.x + 1200) {
        associated.box.x = campos.x + 1200 - associated.box.w - 5;
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
    else if (other.color > 0 && dashing) {
        CollectColor(static_cast<Character::Color>(other.color));
        if (other.blockable >= 0) {
            other.color = -1;
            other.damage = -1;
        }

    }
    else if (other.damage >= 0 && damageCooldown.Get() > 2.0 && invTimer.Get() > 10) {
        float dist = associated.box.y - other.box.y;
        if (abs(dist) < other.box.h/2 + 0.8*(associated.box.h/2)) { // Diminuindo hitbox vertical contra dano
            damageCooldown.Restart();
            if (shield > 0) {
                shield -=1;
            }
            else {
                hp -= other.damage*20;
            }
        }
    }
}

int Character::GetHP() const
{
    return hp;
}

int Character::GetCool() const
{
    return static_cast<int>(spellMixTimer.Get());
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
         int damage;
         int bulcolor;
        if (purpleTimer.Get() < 30.0) {
            damage = 15;
            bulcolor = 3;
        }
        else {
            damage = 10;
            bulcolor = 0;
        }
           
        float maxDistance = 1700.0f;
        bool targetsPlayer = false;
        
        GameObject *spellGO = new GameObject();
        spellGO->box.x = shooterCenter.x;
        spellGO->box.y = shooterCenter.y - 20;
        if (bulcolor == 3 ) {
            spellGO->AddComponent(new Bullet(*spellGO, angle, speed, damage, maxDistance, targetsPlayer, "recursos/img/purpleshot.png",bulcolor));
        }
        else {
            spellGO->AddComponent(new Bullet(*spellGO, angle, speed, damage, maxDistance, targetsPlayer, "recursos/img/Bullet.png",bulcolor));

        }
        
        Game::GetInstance().GetCurrentState().AddObject(spellGO);
        spellTimer.Restart();
    }
}

void Character::ShootMix(Vec2 targetPos, float speed, int damage, float maxDistance, std::string spritePath, int color)
{
    Vec2 shooterCenter = associated.box.GetCenter();
    Vec2 delta = targetPos - shooterCenter;

    if (delta.Magnitude() < 0.01f)
    {
        delta = Vec2(1, 0);
    }
    Vec2 direction = delta.Normalize();

    float angle = atan2(direction.y, direction.x);
    bool targetsPlayer = false;

    GameObject *spell2GO = new GameObject();
    spell2GO->box.x = shooterCenter.x;
    spell2GO->box.y = shooterCenter.y - 20; // Para ajustar a altura do tiro

    // Passa as variáveis  recebidas para o componente Bullet
    spell2GO->AddComponent(new Bullet(*spell2GO, angle, speed, damage, maxDistance, targetsPlayer, spritePath,color));
    Game::GetInstance().GetCurrentState().AddObject(spell2GO);
}

bool Character::IsDashing() {
    return dashing;
}

Rect Character::PlayerBox() {
    return player->associated.box;
}

bool Character::CollectColor(Color newColor)
{
    if (colorInventory.size() >= MAX_COLORS)
    {
        std::cout << "Inventário de cores cheio" << std::endl;
        return false;
    }
    colorInventory.push_back(newColor); // Adiciona a nova cor
    std::cout << "Cor" << newColor << "coletada, Total no inventário: " << colorInventory.size() << std::endl;
    return true;
}

void Character::UseSpell(Vec2 targetPos)
{
    // Se o inventário estiver vazio, toca o som de erro
    if (colorInventory.empty())
    {
        noSpell.Play(1);
        return;
    }

    // Variáveis dependendo da mistura de cores
    std::string spritePath = "recursos/img/Bullet.png"; // Padrão
    float speed = 200.0f;
    int damage = 50;
    float maxDistance = 1200.0f;
    bool shouldShoot = false;
    int col = 0;
    // VErifica as corres do vetor e realiza o diparo/habilidade
    if (colorInventory.size() == 1)
    {
        Color unica = colorInventory[0];

        if (unica == RED)
        {
            spell_red_Sound.Play(1); // Som da magia vermelha
            damage = 50;
            speed = 400.0f;
            spritePath = "recursos/img/redsmall.png"; 
            shouldShoot = true;
        }
        else if (unica == BLUE)
        {
            // fazer a imortalidade
            shouldShoot = false;
            shield = 1;
            shieldTimer.Restart();
            colorInventory.clear();  // Apaga as cores
            spellMixTimer.Restart(); // Reseta o cooldown
        }
    }
    else if (colorInventory.size() == 2)
    {
        Color c1 = colorInventory[0];
        Color c2 = colorInventory[1];
        
        if (c1 == RED && c2 == RED)
        {
            spell_scarlet_Sound.Play(1);
            damage = 150;             
            speed = 450.0f;
            spritePath = "recursos/img/redbig.png";
            shouldShoot = true;
            col = 1;
        }
        else if (c1 == BLUE && c2 == BLUE)
        {
            // O azul duplo por enquanto fica vazio
            shouldShoot = false;
            invTimer.Set(0);
            colorInventory.clear();  // Apaga as cores
            spellMixTimer.Restart(); // Reseta o cooldown
        }
        else if ((c1 == RED && c2 == BLUE) || (c1 == BLUE && c2 == RED))
        {
            spell_purple_Sound.Play(1);
            colorInventory.clear();  // Apaga as cores
            spellMixTimer.Restart(); // Reseta o cooldown
            purpleTimer.Restart(); 
            // damage = 100;
            //speed = 350.0f;
            //maxDistance = 1700.0f;
            //spritePath = "recursos/img/purpleshot.png";
            //shouldShoot = true;
            col = 3;
        }
    }

    // Se a combinação gerou um disparo válido, executa o tiro e limpa as cores
    if (shouldShoot)
    {
        ShootMix(targetPos, speed, damage, maxDistance, spritePath,col);

        colorInventory.clear();  // Apaga as cores
        spellMixTimer.Restart(); // Reseta o cooldown
    }
    
}

std::vector<Character::Color> Character::GetColorInventory() const
{
    return colorInventory;
}