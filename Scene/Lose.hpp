#ifndef LOSE_HPP
#define LOSE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <memory>

class Lose final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit Lose() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void ReplayOnClick();
};

#endif   // LOSESCENE_HPP
