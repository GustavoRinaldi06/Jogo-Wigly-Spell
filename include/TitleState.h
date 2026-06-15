#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "State.h"
#include "Music.h"
#include "Text.h"

class TitleState : public State
{
public:
    TitleState();
    ~TitleState() override;

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;

    // Sem funçõo por enquanto
    void Pause() override {}
    void Resume() override {}

    //Text *pressStart = nullptr; // (DEBUG) para poder acessar e alterar com maior facilidade
private:
    Music backgroundMusic;

    // Guardamos ponteiros para os GameObjects dos botões para verificar o clique no Update
    GameObject *btnContinue = nullptr;
    GameObject *btnNewGame = nullptr;
    GameObject *btnMute = nullptr;
    GameObject *btnExit = nullptr;

    // Ponteiro para o texto de mute para podermos alterá-lo em tempo real
    Text *muteTextPtr = nullptr;

    // Função auxiliar para verificar se o mouse clicou em um GameObject
    bool IsButtonClicked(GameObject *buttonGO);
};

#endif // TITLESTATE_H
