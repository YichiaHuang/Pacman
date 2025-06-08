
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
#include "Scene/MotionScene.hpp"


#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <iostream>

#include <fstream>      // 瑼?頛詨頛詨
#include <chrono>       // ???賊??
#include <ctime>        // C 憸冽???賣
#include <algorithm>    // ??蝞?
#include <string>       // 摮葡??
#include <utility>      // std::pair
#include <filesystem>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

void MotionScene::Initialize() {
    
    ticks=0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    AddNewObject(new Engine::Label("Press Enter to Play", "prstartk.ttf", 80, halfW, halfH / 2 + 50, 10, 155, 155, 255, 0.5, 0.5));
    animationBitmap = al_load_bitmap("Resource/images/cool.png");
if (!animationBitmap) {
    std::cerr << "Failed to load animation.png\n";
}
    
    
   
    
}
void MotionScene::Update(float deltaTime) {
    ticks += deltaTime;
    

if (ticks >= frameDuration) {
    ticks = 1;
    if(animationFrame==5){
        frameCount=(frameCount+1)%3;
    }
    if(animationFrame==4&& frameCount==2){
        animationFrame=2;
        frameCount=0;
    }
    
    animationFrame = (animationFrame + 1) % 6;
    if(animationFrame==0&& frameCount==0){
        animationFrame=2;
    }
    if(animationFrame==0&& frameCount==1){
        animationFrame=2;
    }
}


}

void MotionScene::OnKeyDown(int keyCode) {
    // ?望?憭批?撖怠?瘥摮?
    IScene::OnKeyDown(keyCode);
    
    if(keyCode == ALLEGRO_KEY_ENTER) {
        
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
}
void MotionScene::Terminate() {
    
    IScene::Terminate();
}

void MotionScene::BackOnClick(int stage) {
    
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void MotionScene::Draw() const {
    IScene::Draw();

    if (animationBitmap) {
        int x = 600; // 動畫在畫面上的 X 位置
        int y = 400; // 動畫在畫面上的 Y 位置

        al_draw_scaled_bitmap(
    animationBitmap,
    animationFrame * frameWidth, frameCount * frameHeight, // 從圖片中選取的區域
    frameWidth, frameHeight,
    x, y,
    frameWidth * 2,
    frameHeight * 2,
    0
);
}
}