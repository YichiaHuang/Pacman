#include <iostream>
#include <fstream> 
#include <algorithm>
#include <ctime>
#include <sstream>
#include "ScoreboardScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void ScoreboardScene::Initialize() {
    entryLabels.clear();  // 清除舊的 label 指標，避免重疊

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;

    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 48, halfW, 40, 255, 255, 255, 255, 0.5, 0.5));

    LoadScoreboard();
    pageLabel = new Engine::Label("Page 1", "pirulen.ttf", 28, halfW, 100, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(pageLabel);

    for (int i = 0; i < entriesPerPage; ++i) {
        auto label = new Engine::Label("", "pirulen.ttf", 28, halfW, 180 + i * 60, 255, 255, 255, 255, 0.5, 0.5);
        entryLabels.push_back(label);
        AddNewObject(label);
    }

    auto* btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h - 120, 150, 60);
    btn->SetOnClickCallback([this]() { PrevPageOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 32, halfW - 125, h - 90, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 50, h - 120, 150, 60);
    btn->SetOnClickCallback([this]() { NextPageOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 32, halfW + 125, h - 90, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 75, h - 220, 150, 60);
    btn->SetOnClickCallback([this]() { BackOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 32, halfW, h - 190, 0, 0, 0, 255, 0.5, 0.5));

    UpdatePage();
}

void ScoreboardScene::Terminate() {
    entryLabels.clear();  // 保險清除記錄，避免重疊
    IScene::Terminate();
}

void ScoreboardScene::LoadScoreboard() {
    std::cout << "[DEBUG] Loading scoreboard.txt...\n";

    entries.clear();
    //Open Resource/scoreboard.txt
    std::ifstream fin("Resource/scoreboard.txt");

    if (!fin.is_open()) {
        std::cerr << "[ERROR] Failed to open Resource/scoreboard.txt\n";
        return;
    }

    std::string name, datetime;
    int score;
    while (fin >> name >> score) {
        std::getline(fin, datetime);
        if (!datetime.empty() && datetime[0] == ' ')
            datetime.erase(0, 1);

        std::cout << "[DEBUG] Read: " << name << " " << score << " " << datetime << "\n";

        ScoreEntry entry;
        entry.name = name;
        entry.score = score;
        entry.datetime = datetime;
        entries.push_back(entry);
    }

    fin.close();

    std::sort(entries.begin(), entries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
}


void ScoreboardScene::SaveScoreEntry(const ScoreEntry& entry) {
    // 以附加模式打開 scoreboard.txt 檔案，如果檔案不存在會自動建立
    std::ofstream fout("Resource/scoreboard.txt", std::ios::app);

    // 取得目前時間
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char buffer[64];
    // 將時間格式化成「YYYY-MM-DD HH:MM:SS」的形式並存進 buffer
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);

    fout << entry.name << " " << entry.score << " " << buffer << "\n";
    fout.close();
}

void ScoreboardScene::UpdatePage() {
    int start = currentPage * entriesPerPage;
    for (int i = 0; i < entriesPerPage; ++i) {
        if (start + i < static_cast<int>(entries.size())) {
            const auto& e = entries[start + i];
            std::string dateOnly = e.datetime.substr(0, 10);  // YYYY-MM-DD
            entryLabels[i]->Text = e.name + " | " + std::to_string(e.score) + " | " + dateOnly;
        } else {
            entryLabels[i]->Text = "";
        }
    }
    pageLabel->Text = "Page " + std::to_string(currentPage + 1);
}


void ScoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void ScoreboardScene::NextPageOnClick() {
    if ((currentPage + 1) * entriesPerPage < static_cast<int>(entries.size())) {
        currentPage++;
        UpdatePage();
    }
}

void ScoreboardScene::PrevPageOnClick() {
    if (currentPage > 0) {
        currentPage--;
        UpdatePage();
    }
}
