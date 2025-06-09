#include "SlotMachine.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

SlotMachine::SlotMachine(float x, float y) : x(x+15), y(y) {
    spriteSheet_0 = al_load_bitmap("Resource/images/play/slot_test.png");
    spriteSheet_1 = al_load_bitmap("Resource/images/play/slot_test.png");
    spriteSheet_2 = al_load_bitmap("Resource/images/play/slot_test.png");
    background = al_load_bitmap("Resource/images/play/machine.png");
    if (!spriteSheet_0) {
        std::cerr << "Failed to load slot.png\n";
    }

    
}

SlotMachine::~SlotMachine() {
    if (spriteSheet_0) {
        al_destroy_bitmap(spriteSheet_0);
        al_destroy_bitmap(spriteSheet_1);
        al_destroy_bitmap(spriteSheet_2);
    }
    if (background) {
        al_destroy_bitmap(background);
    }
}

void SlotMachine::StartSpin(){
    for(int i=0; i<3; i++){
        int r=rand() % totalFrames; // 生成隨機數
        start[i]=r;
        cur[i]=r;
        isSpinning[i] = true;
        least[i] = 3; // 每個輪軸至少轉動3次
    }

    int c=rand() % 2;//50% 機率決定是否轉動

    if(c==0){//win
        jackpot = true; // 設置中獎狀態
        int r=rand()%totalFrames;
        for(int i=0; i<3; i++){
            result[i]=r;
        }
    }
    else{//lose
        jackpot = false; // 設置非中獎狀態
        for(int i=0; i<3; i++){
            int r=rand() % totalFrames; // 生成隨機數
            result[i]=r;
       }
       if(result[0] == result[1] && result[1] == result[2]) {
            result[2]= (result[2] + 1) % totalFrames; // 確保不會三個都一樣
        }
    }

}

void SlotMachine::Update(float deltaTime) {
    
    tick += deltaTime;
    if(tick>frameDuration){
        tick=0.0f;
        for(int i=0; i<3; i++){
            if(cur[i]==3)
                least[i]--;
            if(least[i]<=0&&cur[i]==result[i]) {
                isSpinning[i] = false; // 停止轉動
                continue;
            } 
            cur[i]= (cur[i] + 1) % totalFrames; // 更新當前顯示的幀
        }
    }
    if(!isSpinning[0] && !isSpinning[1] && !isSpinning[2]) {
        allStopped = true; // 所有輪軸都停止轉動
    } else {
        allStopped = false; // 還有輪軸在轉動
    }
}

void SlotMachine::Draw() const {
    // 決定每個輪軸顯示的幀
    al_draw_scaled_bitmap(background,
        0, 0,
        185, 152,
        x-10, y-55, 185, 152, 0);

    al_draw_scaled_bitmap(spriteSheet_0,
        0, cur[0] * frameHeight,
        frameWidth, frameHeight,
        x, y, frameWidth, frameHeight, 0);

    al_draw_scaled_bitmap(spriteSheet_1,
        0, cur[1] * frameHeight,
        frameWidth, frameHeight,
        x + 60, y, frameWidth, frameHeight, 0);

    al_draw_scaled_bitmap(spriteSheet_2,
        0, cur[2] * frameHeight,
        frameWidth, frameHeight,
        x + 120, y, frameWidth, frameHeight, 0);
}
