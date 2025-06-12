/*#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include <vector>

namespace Engine {
    class Label;
    class ImageButton;
}

struct ScoreEntry {
    std::string name;
    int score;
    std::string datetime;
};

class ScoreboardScene final : public Engine::IScene {
private:
    std::vector<ScoreEntry> entries;
    std::vector<Engine::Label*> entryLabels;
    Engine::Label* pageLabel;
    int currentPage = 0;
    const int entriesPerPage = 5;

    void LoadScoreboard();
    void SaveScoreEntry(const ScoreEntry& entry);
    void UpdatePage();
    void BackOnClick();
    void NextPageOnClick();
    void PrevPageOnClick();

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
};

#endif // SCOREBOARDSCENE_HPP*/
#ifndef BOARDSECOND_HPP
#define BOARDSECOND_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class BoardSecond final : public Engine::IScene {
public:
    explicit BoardSecond() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void SettingsOnClick(int stage);
    void BackOnClick(int stage);
    void ChangeOnClick(int stage);
    //newwwww
    std::vector<std::tuple<std::string, int, std::string>> sortedScores;
    std::vector<Engine::Label*> scoreLabels;
    int currentPage = 0;
    const int scoresPerPage = 8; // 瘥?憿舐內???豢??
    

   std::vector<std::tuple<std::string, int, std::string>> LoadAndSortScores();
    void DisplayScores();
    void PrevPage();
    void NextPage();
    Engine::Label* pageLabel = nullptr;
};
#endif   // INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
   // SettingsScene_HPP
