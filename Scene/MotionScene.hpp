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
};
#endif   // INC_2025_I2P2_TOWERDEFENSE_WITH_ANSWER_STARTSCENE_H
   // SettingsScene_HPP
