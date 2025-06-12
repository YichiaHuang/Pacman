#ifndef WIN_HPP
#define WIN_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include "UI/Component/TextBox.hpp"

/*class WinScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    //Engine::TextBox* nameBox;

public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
};*/
class Win final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    

public:
    explicit Win() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void ReplayOnClick(int stage);
    void SaveScoreToFile(int score);
    int score=0;
    std::string playerName;
    Engine::Label* nameLabel = nullptr;
    void OnKeyDown(int keyCode) override;
};

#endif   // WINSCENE_HPP
