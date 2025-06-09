#ifndef SLOTMACHINE_HPP
#define SLOTMACHINE_HPP

#include "Engine/Point.hpp"
#include <allegro5/allegro.h>

class SlotMachine {
public:
    explicit SlotMachine(float x, float y);
    ~SlotMachine();
    void Update(float deltaTime);
    void Draw() const;
    void StartSpin(); // 啟動轉動


    float x, y;

    ALLEGRO_BITMAP* spriteSheet_0 = nullptr;
    ALLEGRO_BITMAP* spriteSheet_1 = nullptr;
    ALLEGRO_BITMAP* spriteSheet_2 = nullptr;
    ALLEGRO_BITMAP* background = nullptr;
    
    int totalFrames = 4;
    int frameWidth = 46;
    int frameHeight = 210 / 4;

    // slot state
    bool isSpinning[3] = {false, false, false};
    int start[3];
    int least[3] = {3, 3, 3};
    int result[3] = {0, 0, 0};
    int cur[3] = {0, 0, 0}; // 用於隨機數生成
    float frameDuration = 0.5;
    float tick = 0.0f;
    bool jackpot = false; // 是否中獎
    bool allStopped = false; // 是否所有輪軸都停止轉動
    //bool allStopped = true;
    //bool moving = false; // 是否正在轉動
};
#endif
