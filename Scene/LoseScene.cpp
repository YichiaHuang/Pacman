#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void LoseScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("lose/GameOver.png", halfW, halfH, 600, 600, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Lose :(", "prstartk.ttf", 48, halfW, halfH / 4 + 10, 255, 255, 255, 255, 0.5, 0.5));
    // Replay Button - 左邊
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 250 - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&LoseScene::ReplayOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Replay", "prstartk.ttf", 48, halfW - 250, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    // Back Button - 右邊
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 250 - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&LoseScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "prstartk.ttf", 48, halfW + 250, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    
    bgmInstance = AudioHelper::PlaySample("Pacman/pacman-lose.wav", false, AudioHelper::BGMVolume);
}
void LoseScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void LoseScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void LoseScene::ReplayOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
