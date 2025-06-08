//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

// TODO HACKATHON-2 (1/3): You can imitate the 2 files: 'StartScene.hpp', 'StartScene.cpp' to implement your SettingsScene.
void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Image("start/Pacman_logo.png", halfW - 400, halfH / 3 - 250, 800, 500));

    int buttonW = 400, buttonH = 100;
    int spacing = 40;
    int totalH = 3 * buttonH + 2 * spacing;
    int baseY = halfH - totalH / 2 + 80;

    int positions[3] = {
        baseY,                          // Play
        baseY + buttonH + spacing,      // Settings
        baseY + 2 * (buttonH + spacing) // Info
    };

    // Play Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, positions[0], buttonW, buttonH);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "prstartk.ttf", 48, halfW, positions[0] + 50, 0, 0, 0, 255, 0.5, 0.5));

    // Settings Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, positions[1], buttonW, buttonH);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "prstartk.ttf", 48, halfW, positions[1] + 50, 0, 0, 0, 255, 0.5, 0.5));

    // Info Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, positions[2], buttonW, buttonH);
    btn->SetOnClickCallback([=](){ Engine::GameEngine::GetInstance().ChangeScene("info"); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Info", "prstartk.ttf", 48, halfW, positions[2] + 50, 0, 0, 0, 255, 0.5, 0.5));

}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}