#ifndef SCOREBOARDSCENE_HPP
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

#endif // SCOREBOARDSCENE_HPP
