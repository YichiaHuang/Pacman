/*#include <iostream>
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

    AddNewObject(new Engine::Label("Scoreboard", "prstartk.ttf", 48, halfW, 40, 255, 255, 255, 255, 0.5, 0.5));

    LoadScoreboard();
    pageLabel = new Engine::Label("Page 1", "prstartk.ttf", 28, halfW, 100, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(pageLabel);

    for (int i = 0; i < entriesPerPage; ++i) {
        auto label = new Engine::Label("", "prstartk.ttf", 28, halfW, 180 + i * 60, 255, 255, 255, 255, 0.5, 0.5);
        entryLabels.push_back(label);
        AddNewObject(label);
    }

    //Prev Button
    auto* btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h - 120, 150, 60);
    btn->SetOnClickCallback([this]() { PrevPageOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "prstartk.ttf", 32, halfW - 125, h - 90, 0, 0, 0, 255, 0.5, 0.5));

    //Next Button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 50, h - 120, 150, 60);
    btn->SetOnClickCallback([this]() { NextPageOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "prstartk.ttf", 32, halfW + 125, h - 90, 0, 0, 0, 255, 0.5, 0.5));
   
    //Back Button
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 75, h - 220, 150, 60);
    btn->SetOnClickCallback([this]() { BackOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "prstartk.ttf", 32, halfW, h - 190, 0, 0, 0, 255, 0.5, 0.5));

    UpdatePage();
}

void ScoreboardScene::Terminate() {
    entryLabels.clear();
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
*/



#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
//newwwww
#include <fstream>
#include <vector>
#include <algorithm>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/ScoreboardScene.hpp"


void ScoreboardScene::Initialize() {
    

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Label("ScoreBoard", "prstartk.ttf", 80, halfW, halfH / 5 + 50, 10, 155, 155, 255, 0.5, 0.5));
    

   
    DisplayScores();


    btn = new Engine::ImageButton( "stage-select/floor.png", "stage-select/bott.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "prstartk.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));


    
    btn = new Engine::ImageButton( "stage-select/floor.png", "stage-select/bott.png", halfW - 200-450, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPage, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "prstartk.ttf", 48, halfW-450, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton( "stage-select/floor.png", "stage-select/bott.png", halfW - 200+450, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextPage, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "prstartk.ttf", 48, halfW+450, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
}
void ScoreboardScene::Terminate() {
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick(int stage) {
    currentPage = 0; 
    sortedScores.clear();

    for (auto& label : scoreLabels) {
        RemoveObject(label->GetObjectIterator());
    }
    scoreLabels.clear();

    if (pageLabel) {
        RemoveObject(pageLabel->GetObjectIterator());
        pageLabel = nullptr;
    }
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
//newwwww
void ScoreboardScene::PrevPage() {
    if (currentPage > 0) {
        currentPage--;
        DisplayScores();
    }
}

void ScoreboardScene::NextPage() {
    if ((currentPage + 1) * scoresPerPage < sortedScores.size()) {
        currentPage++;
        DisplayScores();
    }
}



std::vector<std::tuple<std::string, int, std::string>> ScoreboardScene::LoadAndSortScores() {
    std::vector<std::tuple<std::string, int, std::string>> scores;
    std::ifstream scoreFile("Resource/scoreboard.txt");

    if (scoreFile.is_open()) {
        std::string line;
        while (std::getline(scoreFile, line)) {
            size_t firstDelim = line.find('|');
            size_t secondDelim = line.find('|', firstDelim + 1);
            if (firstDelim != std::string::npos && secondDelim != std::string::npos) {
                try {
                    std::string name = line.substr(0, firstDelim);
                    int score = std::stoi(line.substr(firstDelim + 1, secondDelim - firstDelim - 1));
                    std::string time = line.substr(secondDelim + 1);
                    scores.emplace_back(name, score, time);
                } catch (...) {
                }
            }
        }
        scoreFile.close();
    }

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    return scores;
}



void ScoreboardScene::DisplayScores() {
    for (auto& label : scoreLabels) {
        RemoveObject(label->GetObjectIterator());
    }
    scoreLabels.clear();

    if (pageLabel) {
        RemoveObject(pageLabel->GetObjectIterator());
        pageLabel = nullptr;
    }

    
    sortedScores = LoadAndSortScores();
    

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int startY = h / 4 + 50;

    std::string pageInfo = "Page " + std::to_string(currentPage + 1) + 
                         " of " + std::to_string((sortedScores.size() + scoresPerPage - 1) / scoresPerPage);
    pageLabel = new Engine::Label(pageInfo, "prstartk.ttf", 32, halfW, startY - 40, 200, 200, 255, 255, 0.5, 0.5);
    AddNewObject(pageLabel);

    int startIdx = currentPage * scoresPerPage;
    int endIdx = std::min(startIdx + scoresPerPage, (int)sortedScores.size());

    
   for (int i = startIdx; i < endIdx; i++) {
        const auto& scoreTuple = sortedScores[i];
        std::string scoreText = std::to_string(i + 1) + ". " +
                                std::get<0>(scoreTuple) + "  " + // ??
                                std::to_string(std::get<1>(scoreTuple)) + " " + // ?
                                std::get<2>(scoreTuple); // ??

        Engine::Label* label = new Engine::Label(
        scoreText, "prstartk.ttf", 36,
        halfW, startY + (i - startIdx) * 50,
        255, 255, 255, 255, 0.5, 0.5);

        AddNewObject(label);
        scoreLabels.push_back(label);
    }

    if (sortedScores.empty()) {
        AddNewObject(new Engine::Label(
            "No scores recorded yet!", "prstartk.ttf", 36, 
            halfW, startY, 255, 255, 255, 255, 0.5, 0.5));
    }
}