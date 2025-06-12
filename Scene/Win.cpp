/*#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include "UI/Component/TextBox.hpp"
#include "Engine/Resources.hpp"

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "prstartk.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "prstartk.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");

    // ALLEGRO_FONT* font = Engine::Resources::GetInstance().GetFont("prstartk.ttf", 32).get();
    // nameBox = new Engine::TextBox("Enter your name", font, halfW - 150, halfH, 300, 50);

    // AddNewControlObject(nameBox);
    // AddNewObject(nameBox);
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
*/
#include <functional>
#include <string>
#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <memory>
#include <queue>
#include <vector>
#include <iostream>

#include <fstream>      // 瑼?頛詨頛詨
#include <chrono>       // ???賊??
#include <ctime>        // C 憸冽???賣
#include <algorithm>    // ??蝞?
#include <string>       // 摮葡??
#include <utility>      // std::pair
#include <filesystem>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Win.hpp"

void Win::Initialize() {
    playerName.clear();
    PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    if (playScene) {
        score = playScene->GetMoney();
    }
   // SaveScoreToFile(score);


    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("ghost/pacman-ghosts.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "prstartk.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    /*btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");*/

    playerName = "";
    int nameInputY = halfH / 4 + 60;    
    nameLabel = new Engine::Label("Name: ", "prstartk.ttf", 36, halfW, nameInputY, 255, 255, 150, 255, 0.5, 0.5);
    AddNewObject(nameLabel);
    
}
void Win::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void Win::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
/*void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}*/




void Win::OnKeyDown(int keyCode) {
    // ?望?憭批?撖怠?瘥摮?
    IScene::OnKeyDown(keyCode);
    if ((keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) ||
        (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9)) {
        char c;
        if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z)
            c = 'A' + (keyCode - ALLEGRO_KEY_A);
        else
            c = '0' + (keyCode - ALLEGRO_KEY_0);
        playerName += c;
    }
    // Backspace
    if (keyCode == ALLEGRO_KEY_BACKSPACE && !playerName.empty()) {
        playerName.pop_back();
    }
    // ?憭?0摮?
    if (playerName.size() > 10)
        playerName.resize(10);

    // ?湔憿舐內
    if (nameLabel)
        nameLabel->Text = "Name: " + playerName;
    if(keyCode == ALLEGRO_KEY_ENTER) {
        SaveScoreToFile(score);
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
}
void Win::SaveScoreToFile(int score) {
    std::string filePath = "Resource/score_2.txt";
    std::ofstream scoreFile(filePath, std::ios::app);

    // ???嗅???
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

    // 撖怠?????貉???
    std::string nameToSave = playerName.empty() ? "Anonymous" : playerName;
    scoreFile << nameToSave << "|" << score << "|" << timeStr << "\n";
    scoreFile.close();

    std::cout << "[SaveScoreToFile] Score saved: " << nameToSave << " " << score << " at " << timeStr << std::endl;
}