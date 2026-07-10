#include "DiscoGhost.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "State.h"
#include "Camera.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "HallwayState.h"
#include "Missile.h"
#include "bounceBall.h"
#include "WavyNote.h"
#include "BeatWave.h"
#include "DanceGhost.h"
#include "Smoke.h"

#include "Bullet.h"
#include "Character.h"
#include "Text.h"


#include <iostream>

DiscoGhost::DiscoGhost(GameObject &associated, const std::string &spritePath)
    : Component(associated)
{
    associated.layer = 4.5;
    associated.damage = 1;

    auto renderer = new SpriteRenderer(associated, spritePath, 10, 7);

    associated.AddComponent(renderer);

    renderer->SetScale(1.0, 1.0);
    renderer->SetCameraFollower(0.0);

    // Novos sons
    // hitSound = Sound();
    // deathSound = Sound();
    // fallSound = Sound();

    // Cria as animações

    auto animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 19, (1.0/24)));
    animator->AddAnimation("prepgrav", Animation(29, 35, (1.0/24)));
    animator->AddAnimation("downgrav", Animation(36, 39, (1.0/24)));
    animator->AddAnimation("upgrav", Animation(42, 45, (1.0/24)));
    animator->AddAnimation("recdowngrav", Animation(40, 41,(1.0/24)));
    animator->AddAnimation("recupgrav", Animation(46, 47, (1.0/24)));
    animator->AddAnimation("summon", Animation(20, 28, (1.0/24)));
    animator->AddAnimation("smnrecover", Animation(25, 28, (1.0/24)));
    animator->AddAnimation("disco", Animation(52, 59, (1.0/24)));
    animator->AddAnimation("discorecover", Animation(59, 60, (1.0/24)));
    animator->AddAnimation("death", Animation(61, 68, (1.0/24)));
    associated.AddComponent(animator);
    animator->SetAnimation("idle");

    Collider *col = new Collider(associated, Vec2(Vec2(0.4, 0.8)));
    associated.AddComponent(col);
    // associated.AddComponent(new Collider(associated,Vec2(1,1)));
    deathTimer.Restart();
    specialInvuln.Restart();
    waveCD.Restart();
    health = 1250;
    dead = false;
    ATK = -1;
    SmnTimer.Set(9999);

    Error = Sound("recursos/audio/Erro.mp3");
    Right_Place = Sound("recursos/audio/Right_Place.mp3");
}

DiscoGhost::~DiscoGhost()
{
}

void DiscoGhost::Start()
{
    // TExto do ataque do disco(debug ou possível uso)
    SDL_Color white = {255, 255, 255, 255};
    GameObject *textGO = new GameObject();
    textGO->layer = 11;
    textGO->box.x = 350;
    textGO->box.y = 67;

    // inicia com a string vazia
    discoInfoText = new Text(*textGO, "recursos/font/heavy heap.otf", 28, BLENDED, "", white);
    textGO->AddComponent(discoInfoText);
    discoInfoText->SetCameraFollower(true);

    Game::GetInstance().GetCurrentState().AddObject(textGO);
    ATK = -1;
}

void DiscoGhost::Update(float dt)
{
    GameData::bossHP = health;
    if (health <= 0)
    {
        dead = true;
        health = 0;
    }
    // Ao morrer -------------------------------------------------------------------------------
    if (dead)
    {
        // dispara animação e som apenas uma vez
        Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
        associated.damage = -1;
        if (!deathAnimTriggered)
        {
            deathAnimTriggered = true;
            // fallSound.Play(1);

            // seta animação "popping"

            if (animator)
                animator->SetAnimation("death");
                animcounter = 10;
            deathTimer.Restart();
        }

        // avança timer de morte
        deathTimer.Update(dt);

        // só deleta após 0.5s
        if (animator->GetAnimation() == "death" && animator->wrapped){
            animcounter--;
            animator->wrapped = false;
            if (animcounter <= 0) {
                associated.RequestDelete();
            }
            
        }
        return; // não executa mais lógica de movimento
    }
    

    Animator *animator = static_cast<Animator *>(associated.GetComponent("Animator"));
    NoteTimer.Update(dt);
    specialInvuln.Update(dt);
    AnimTimer.Update(dt);
    SmnTimer.Update(dt);
    DiscoTimer.Update(dt);
    waveCD.Update(dt);
    if (GameData::finalfase)
    {
        noteTime = 1.2;
        atkTime = 3;
    }
    if (NoteTimer.Get() > noteTime)
    {
        NoteTimer.Restart();
        if (!GameData::finalfase)
        {
            if (!GameData::discostart) {
                noteTime = 4;
            }
            else if (health > 750)
            {
                noteTime = 6 + rand() % 5;
                ;
            }
            else
            {
                noteTime = 4 + rand() % 3;
            }

            int count = 1;
            for (int i = 0; i < count; i++)
            {
                int colorchk = (rand() % 100);
                int color = 0;
                if (colorchk >= 50)
                {
                    if (last_color == 0)
                    {
                        colorchk -= 50;
                    }
                    else
                    {
                        last_color = 0;
                        color = 0;
                    }
                }
                if (colorchk < 25)
                {
                    color = 1;
                    if (last_color == 1)
                    {
                        color = 2;
                        last_color = 2;
                    }
                    else {
                        color = 1;
                        last_color = 1;
                    }
                }
                else if (colorchk < 50)
                {
                    color = 2;
                    if (last_color == 2)
                    {
                        color = 1;
                        last_color = 1;
                    }
                    else {
                        color = 2;
                        last_color = 2;
                    }
                }
                if (!GameData::discostart) {
                    color = 0;
                    last_color = 0;
                }
                NoteATK(color);
            }
        }
        else
        {
            if (waveCD.Get() > 1.5)
            {
                WaveATK(nextwave);
                nextwave = (nextwave + 1) % 2;
            }
        }
    }
    if (!GameData::discostart)
    {
        return;
    }

    if (ATK == -1)
    {
        // Inicialização do Ataque de Disco (tutorial)
        if (!attacked)
        {
            attacked = true;
            discoCurrentRound = -2;
            roundActive = false;

            GameData::discoAttackActive = true;
            GameData::discoBlackout = true;

            // Atualiza o texto
            if (discoInfoText != nullptr)
            {
                discoInfoText->SetText("VAMOS VER SE VOCE SABE DANCAR! DJ TOCA O SOM...");
            }

            discoRoundTimer.Restart();
        }
        else
        {
            discoRoundTimer.Update(dt);

            if (discoCurrentRound == -2)
            {
                float delayAntesDeMudarModo = 2.0f;

                if (discoRoundTimer.Get() >= delayAntesDeMudarModo)
                {
                    // levanta o jogador
                    Character::player->SetSpeedY(-900.0f);

                    // função dj
                    DiscoState *currentState = dynamic_cast<DiscoState *>(&Game::GetInstance().GetCurrentState());
                    if (currentState != nullptr)
                    {
                        currentState->DontStopTheMusic();
                    }

                    discoCurrentRound = -1;
                    discoRoundTimer.Restart();
                }
            }

            if (discoCurrentRound == -1) // Adiciona tempo de "voo"
            {
                float delayAntesDeMudarModo = 0.4f;

                if (discoRoundTimer.Get() >= delayAntesDeMudarModo)
                {
                    GameData::gameMode = 0;
                    animator->SetAnimation("idle");

                    discoCurrentRound = 0;
                    discoRoundTimer.Restart();
                }
            }

            // Tempo de respiro até inicio do primeiro round
            if (discoCurrentRound == 0)
            {
                float delayNaParede = 1.0f;
                if (discoRoundTimer.Get() >= delayNaParede)
                {
                    discoInfoText->SetText("ACERTE O PASSO DE DANCA E EU TE LIBERO!");
                    if (discoRoundTimer.Get() >= 2.5f)
                    {
                        discoCurrentRound = 1; // Começa o Round de verdade!
                        roundActive = false;
                    }
                }
            }

            // Inicio de um novo round tutorial, escolhe a cor, sinaliza e espera
            if (!roundActive && (discoCurrentRound <= 1 && discoCurrentRound > 0))
            {
                roundActive = true;
                discoRoundTimer.Restart();

                if (GameData::danceFloorPtr != nullptr)
                {
                    GameData::danceFloorPtr->ActivateDisco();
                }
                GameData::discoError = false;

                // Escolhe um valor de cor randômico entre 1 e 4
                targetColor = 0 + (rand() % 4);

                GameData::targetDiscoColor = targetColor;
                GameData::discoAttackActive = true;
                GameData::discoBlackout = false;

                // Texto da cor para guiar o player
                std::string corNome;
                if (targetColor == 0)
                    corNome = "ROSA";
                else if (targetColor == 1)
                    corNome = "AMARELO";
                else if (targetColor == 2)
                    corNome = "AZUL";
                else
                    corNome = "VERDE";

                if (discoInfoText != nullptr)
                {
                    discoInfoText->SetText("MOSTRE QUE SABE DANCAR VA PARA A COR " + corNome + "!");
                }
            }

            // Fim do tempo do Round Atual
            if (roundActive && !feedbackActive && discoRoundTimer.Get() >= 2.5f)
            {
                feedbackActive = true;
                discoRoundTimer.Restart(); // Reinicia o timer para contar os 2 segundos de respiro
                GameData::discoBlackout = true;

                if (Character::player != nullptr)
                {
                    if (GameData::danceFloorPtr != nullptr)
                    {
                        // posição X e Y do player
                        float px = Character::player->GetGameObject()->box.GetCenter().x;
                        float py = Character::player->GetGameObject()->box.GetCenter().y;

                        int playerColor = GameData::danceFloorPtr->GetColorAtPosition(px, py);

                        // Se o jogador não estiver na cor certa, aplica dano
                        if (playerColor != targetColor)
                        {
                            GameData::danceFloorPtr->Error(); // Pista fica inteira vermelha

                            GameData::discoError = true;
                            GameData::discoBlackout = false;
                            if (discoInfoText != nullptr) // Texto boss
                            {
                                Error.Play();
                                discoInfoText->SetText("ERROU! TENTE NOVAMENTE NO PROXIMA PASSO!");
                            }
                        }
                        // Se o jogador estiver na cor certa
                        else
                        {
                            if (discoInfoText != nullptr)
                            {
                                Right_Place.Play();
                                discoInfoText->SetText("MUITO BEM! AGORA SEI QUE VOCE SABE DANCAR...");
                                acertou = true;
                            }
                        }
                    }
                }
            }

            // Tempo de respiro mostrando o resultado
            if (feedbackActive && discoRoundTimer.Get() >= 2.0f)
            {
                feedbackActive = false; // Desliga o feedback
                roundActive = false;    // Desativa o round atual para disparar o sorteio do próximo

                if (acertou)
                {
                    // Retira o texto
                    if (discoInfoText != nullptr)
                    {
                        discoInfoText->SetText("");
                    }

                    // função dj
                    DiscoState *currentState = dynamic_cast<DiscoState *>(&Game::GetInstance().GetCurrentState());
                    if (currentState != nullptr)
                    {
                        currentState->DontStopTheMusic();
                    }

                    // Retorna as configurações padrões do jogo
                    GameData::gameMode = 1;
                    GameData::inverted = false;
                    GameData::discoAttackActive = false;
                    GameData::discoBlackout = false;
                    

                    // Reseta estados do Boss
                    ATK = 0;
                    animator->SetAnimation("idle");
                    DiscoTimer.Restart();
                    discoTime = 20 + rand() % 40;
                }
            }
        }
    }
    if (ATK == 0)
    {
        if (health <= 300)
        {
            GameData::finalfase = true;
        }
        AtkTimer.Update(dt);
        if (AtkTimer.Get() > atkTime)
        {
            AtkTimer.Restart();
            if (!GameData::finalfase)
            {
                if (health > 750)
                {
                    atkTime = 2 + rand() % 2;
                }
                else
                {
                    atkTime = 1.5 + (rand() % 2) / 2;
                }

                swapcount -= 1;

                if (swapcount <= 0)
                {
                    swapcount = 2 + (rand() % 2);
                    ATK = 2;
                    animator->SetAnimation("prepgrav");

                    GameData::inversedisco = 1;

                    AnimTimer.Restart();
                    attacked = false;
                }
                else
                {
                    int choice = rand() % 100;
                    if (DiscoTimer.Get() > discoTime && choice < 50)
                    {
                        ATK = 4;
                        animator->SetAnimation("disco");
                        attacked = false;
                    }
                    else if (SmnTimer.Get() > smnTime && choice < 75 && choice >= 25)
                    {
                        ATK = 3;
                        animator->SetAnimation("summon");
                        attacked = false;
                    }
                    else
                    {
                        ATK = 1;
                        animator->SetAnimation("summon");
                        attacked = false;
                    }
                }
            }
            else
            {
                atkTime = 2;
                ATK = 2;
                animator->SetAnimation("prepgrav");
                AnimTimer.Restart();
                attacked = false;
            }
        }
    }
    else if (ATK == 1)
    {

        if (!attacked && animator->GetCurrentFrame() == 25)
        {
            WaveATK(0);
            WaveATK(1);
            waveCD.Restart();
            attacked = true;
            animator->SetAnimation("smnrecover");
            animcounter = 5;
            AnimTimer.Restart();
        }
        else if (attacked && animator->GetAnimation() == "smnrecover" && animator->wrapped)
        {
            animcounter--;
            if (animcounter <= 0)  {
                ATK = 0;
                animator->SetAnimation("idle");
            }
            else {
                animator->wrapped = false;
            }
            
        }
    }
    else if (ATK == 2)
    {
        if (attacked && (animator->GetAnimation() == "recupgrav" || animator->GetAnimation() == "recdowngrav") && animator->wrapped)
        {
            ATK = 0;
            AnimTimer.Restart();
            animator->SetAnimation("idle");
        }
        else if (!attacked && AnimTimer.Get() > 1.5)
        {
            if (GameData::inversedisco > 0)
            {
                GameData::inversedisco = 2;
            }
            if (GameData::inverted)
            {
                animator->SetAnimation("downgrav");
                AnimTimer.Restart();
            }
            else
            {
                animator->SetAnimation("upgrav");
                AnimTimer.Restart();
            }
        }

        if (!attacked && !GameData::inverted && animator->GetCurrentFrame() == 45 && animator->wrapped) 
        {
            attacked = true;
            GameData::inverted = true; 
            AnimTimer.Restart();
            animator->SetAnimation("recupgrav");
        }
        if (!attacked && GameData::inverted && animator->GetCurrentFrame() == 39 && animator->wrapped)
        {
            attacked = true;
            GameData::inverted = false;
            AnimTimer.Restart();
            animator->SetAnimation("recdowngrav");
        }
    }
    else if (ATK == 3)
    {
        if (!attacked && animator->GetCurrentFrame() == 25)
        {
            int pos = rand() % 2;
            int startpos = pos;
            bool exit = false;
            while (GameData::summonalive[pos] || GameData::summonalive[3 - pos])
            {
                pos = (pos + 1) % 2;
                if (pos == startpos)
                {
                    exit = true;
                    break;
                }
            }
            if (!exit)
            {
                GameData::summonalive[pos] = true;
                GameData::summonalive[3 - pos] = true;
                SmnATK(0, pos);
                SmnATK(1, pos);
                attacked = true;
                animator->SetAnimation("smnrecover");
                animcounter = 5;
                AnimTimer.Restart();
            }
            else
            {
                swapcount = 2 + (rand() % 2);
                ATK = 2;
                animator->SetAnimation("prepgrav");

                GameData::inversedisco = 1;

                AnimTimer.Restart();
                attacked = false;
            }
        }
        else if (attacked && AnimTimer.Get() > 0.25)
        {
            ATK = 0;
            animator->SetAnimation("idle");
            SmnTimer.Restart();
            smnTime = 15 + rand() % 30;
        }
    }
    else if (ATK == 4)
    {
        // Inicialização do Ataque de Disco (Round 1)
        if (!attacked)
        {
            attacked = true;
            discoCurrentRound = -1;
            roundActive = false;

            GameData::discoAttackActive = true;
            GameData::discoBlackout = true;

            // levanta o jogador
            if (GameData::inverted == false)
                Character::player->SetSpeedY(-900.0f);
            else
                Character::player->SetSpeedY(900.0f);

            // Atualiza o texto
            if (discoInfoText != nullptr)
            {
                discoInfoText->SetText("PREPARE-SE PARA DANCAR! DJ TOCA O SOM...");
            }

            // função dj
            DiscoState *currentState = dynamic_cast<DiscoState *>(&Game::GetInstance().GetCurrentState());
            if (currentState != nullptr)
            {
                currentState->DontStopTheMusic();
            }

            discoRoundTimer.Restart();
        }
        else
        {
            discoRoundTimer.Update(dt);

            if (discoCurrentRound == -1) // Adiciona tempo de "voo"
            {
                float delayAntesDeMudarModo = 0.4f;

                if (discoRoundTimer.Get() >= delayAntesDeMudarModo)
                {
                    GameData::gameMode = 0;
                    animator->SetAnimation("idle");

                    discoCurrentRound = 0;
                    discoRoundTimer.Restart();
                }
            }

            // Tempo de respiro até inicio do primeiro round
            if (discoCurrentRound == 0)
            {
                float delayNaParede = 1.0f;
                if (discoRoundTimer.Get() >= delayNaParede)
                {
                    discoCurrentRound = 1; // Começa o Round de verdade!
                    roundActive = false;
                }
            }

            // Inicio de um novo round 1-3, escolhe a cor, sinaliza e espera
            if (!roundActive && (discoCurrentRound <= 3 && discoCurrentRound > 0))
            {
                roundActive = true;
                discoRoundTimer.Restart();

                if (GameData::danceFloorPtr != nullptr)
                {
                    GameData::danceFloorPtr->ActivateDisco();
                }

                GameData::discoError = false;

                // Escolhe um valor de cor randômico entre 1 e 4
                targetColor = 0 + (rand() % 4);
                GameData::targetDiscoColor = targetColor;

                GameData::discoBlackout = false;

                // Texto da cor para guiar o player
                std::string corNome;
                if (targetColor == 0)
                    corNome = "ROSA";
                else if (targetColor == 1)
                    corNome = "AMARELO";
                else if (targetColor == 2)
                    corNome = "AZUL";
                else
                    corNome = "VERDE";

                if (discoInfoText != nullptr)
                {
                    discoInfoText->SetText("ROUND " + std::to_string(discoCurrentRound) + ": VA PARA O " + corNome + "!");
                }
            }

            // Fim do tempo do Round Atual
            if (roundActive && !feedbackActive && discoRoundTimer.Get() >= 2.5f)
            {
                feedbackActive = true;
                discoRoundTimer.Restart(); // Reinicia o timer para contar os 2 segundos de respiro
                GameData::discoBlackout = true;

                if (Character::player != nullptr)
                {
                    if (GameData::danceFloorPtr != nullptr)
                    {
                        // posição X e Y do player
                        float px = Character::player->GetGameObject()->box.GetCenter().x;
                        float py = Character::player->GetGameObject()->box.GetCenter().y;

                        int playerColor = GameData::danceFloorPtr->GetColorAtPosition(px, py);

                        // Se o jogador não estiver na cor certa, aplica dano
                        if (playerColor != targetColor)
                        {
                            Error.Play();
                            Character::player->ApplyDamage(20); // Dá 20 de dano
                            
                            GameData::danceFloorPtr->Error();   // Pista fica inteira vermelha
                            GameData::discoError = true;
                            GameData::discoBlackout = false;

                            if (discoInfoText != nullptr) // Texto boss
                            {
                                discoInfoText->SetText("ERROU! TENTE NOVAMENTE NA PROXIMA DANCA!");
                            }
                            std::cout << "Player errou a cor! Levou dano.\n";
                        }
                        // Se o jogador estiver na cor certa
                        else
                        {
                            Right_Place.Play();
                            if (discoInfoText != nullptr)
                            {
                                discoInfoText->SetText("MUITO BEM! VOCE E BOM DE DANCA!");
                            }
                            std::cout << "[Boss Disco] Player acertou a cor protegida!\n";
                        }
                    }
                }
            }

            // Tempo de respiro mostrando o resultado
            if (feedbackActive && discoRoundTimer.Get() >= 1.5f)
            {
                feedbackActive = false; // Desliga o feedback
                roundActive = false;    // Desativa o round atual para disparar o sorteio do próximo

                // Incrementa para avançar de rodada
                discoCurrentRound++;

                // Se completou os 3 rounds, encerra o ataque aqui dentro
                if (discoCurrentRound > 3)
                {
                    // Retira o texto
                    if (discoInfoText != nullptr)
                    {
                        discoInfoText->SetText("");
                    }

                    GameData::discoAttackActive = false;
                    GameData::discoBlackout = false;

                    // Retorna as configurações padrões do jogo
                    GameData::gameMode = 1;
                    GameData::inverted = false;

                    // função dj
                    DiscoState *currentState = dynamic_cast<DiscoState *>(&Game::GetInstance().GetCurrentState());
                    if (currentState != nullptr)
                    {
                        currentState->DontStopTheMusic();
                    }

                    // Reseta estados do Boss
                    ATK = 0;
                    animator->SetAnimation("idle");
                    DiscoTimer.Restart();
                    discoTime = 20 + rand() % 40;
                }
            }
        }
    }
}

void DiscoGhost::Render() {}

bool DiscoGhost::Is(const std::string &type)
{
    return type == "DiscoGhost";
}

void DiscoGhost::NoteATK(int color)
{
    GameObject *noteGO = new GameObject();
    noteGO->box.x = associated.box.x + associated.box.w;
    if (!GameData::discostart) {
        //
        noteGO->box.x = Camera::GetInstance().GetPosition().x + 1200;
    }
    int notepos = (rand() % 8) * 60;
    while (notepos == lastnote)
    {
        notepos = (rand() % 8) * 60;
    }
    lastnote = notepos;
    noteGO->box.y = notepos + 120; // Centro do mapa

    noteGO->AddComponent(new WavyNote(*noteGO, "recursos/img/notamus.png", color)); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(noteGO);
}

void DiscoGhost::WaveATK(int side)
{
    GameObject *waveGO = new GameObject();
    waveGO->box.x = associated.box.x + associated.box.w;                            // Centro do mapa
    waveGO->box.y = +530;                                                         // Centro do mapa
    waveGO->AddComponent(new BeatWave(*waveGO, "recursos/img/BeatWave.png", side)); // substitua pela imagem correta
    SpriteRenderer *waveSprite = (SpriteRenderer *)waveGO->GetComponent("SpriteRenderer");
    if (waveSprite != nullptr)
    {
        waveSprite->SetScale(2.0f, 2.0f);
    }
    Game::GetInstance().GetCurrentState().AddObject(waveGO);
}

void DiscoGhost::SmnATK(int side, int pos)
{
    GameObject *ghostGO = new GameObject();
    ghostGO->box.x = associated.box.x + associated.box.w; // Centro do mapa
    if (side == 0)
    {
        ghostGO->box.y = pos * 150 + 60; // Centro do mapa
    }
    else
    {
        ghostGO->box.y = 520 - pos * 150; // Centro do mapa
    }
    int position = pos;
    if (side > 0)
    {
        position = 3 - pos;
    }
    ghostGO->AddComponent(new DanceGhost(*ghostGO, "recursos/img/minionghost.png", position)); // substitua pela imagem correta
    Game::GetInstance().GetCurrentState().AddObject(ghostGO);
}

void DiscoGhost::NotifyCollision(GameObject &other)
{
    if (!GameData::discostart)
    {
        return;
    }
    Collider *collider = (Collider *)other.GetComponent("Collider");
    if (collider && collider->tag == "bullet")
    {
        if (!dead)
        {
            Bullet *bul = (Bullet *)other.GetComponent("Bullet");
            if (specialInvuln.Get() > 2.0 || bul->bulletcolor == 0 || bul->bulletcolor == 3)
            {
                health -= bul->damage;
                if (bul->bulletcolor == 0 || bul->bulletcolor == 3)
                {
                    other.RequestDelete();
                    other.StandardSmoke();
                }
                else
                {
                    if (other.damage > 100) {
                        other.StandardSmoke(Vec2(3.0,3.0));
                    }
                    else {
                        other.StandardSmoke(Vec2(2.0,2.0));
                    }
                    specialInvuln.Restart();
                }
                
            }
        }
    }
}
