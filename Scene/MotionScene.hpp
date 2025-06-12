#ifndef MOTIONSCENE_HPP
#define MOTIONSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"
class MotionScene final : public Engine::IScene {
public:
    explicit MotionScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void SettingsOnClick(int stage);
    void BackOnClick(int stage);
    //newwwww

    void Update(float deltaTime) override;
    float ticks;
    void OnKeyDown(int keyCode) override;

    ALLEGRO_BITMAP* animationBitmap = nullptr;
    int totalFrames = 5;        // 幀數
    int frameWidth = 160;       // 每幀寬
    int frameHeight = 160;       // 每幀高
    float frameDuration = 1.5; // 每幀持續時間
    void Draw() const override;
    int animationFrame=2;
    int frameCount = 0; // 當前幀數
    bool transitionStarted = false;
    float transitionTimer = 0.0f;
    const float ghostDelay = 0.5f;
    const float ghostSpeed = 300.0f;
    mutable std::vector<float> ghostX = { -100, -100, -100, -100 };
    mutable std::vector<bool> ghostDone = { false, false, false, false };
    mutable ALLEGRO_BITMAP* ghostBitmap = nullptr;

};
#endif
