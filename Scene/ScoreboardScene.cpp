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

    AddNewObject(new Engine::Label("ScoreBoard--      ", "prstartk.ttf", 80, halfW, halfH / 5 + 50, 10, 155, 155, 255, 0.5, 0.5));
    

   
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

    btn = new Engine::ImageButton( "stage-select/floor.png", "stage-select/bott.png", halfW - 200+450, halfH * 1 / 4 - 50+30, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage1", "prstartk.ttf", 48, halfW+450, halfH * 1 / 4+30, 0, 0, 0, 255, 0.5, 0.5));
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

void ScoreboardScene::ChangeOnClick(int stage){
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
    Engine::GameEngine::GetInstance().ChangeScene("score_second");
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