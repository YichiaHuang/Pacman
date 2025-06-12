#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>

#include "UI/Component/ImageButton.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"

#include "Engine/IScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Scene/SecondScene.hpp"
#include "UI/Component/Label.hpp"
#include "Dot/Dot.hpp"
#include "Pacman/Pac.hpp"
#include "Dot/NormalDot.hpp"
#include "Dot/PD.hpp"
#include "Ghost/Blinky.hpp"
#include "Ghost/Inky.hpp"
#include "Ghost/Pinky.hpp"
#include "Ghost/Clyde.hpp"
#include "Dot/Ice.hpp"
#include "Dot/Speed.hpp"
#include "Dot/Star.hpp"
#include "Dot/Scroll.hpp"
#include <allegro5/allegro_primitives.h>
#include "GhostSecond/Blin.hpp"
#include "GhostSecond/GhostSecond.hpp"
#include "GhostSecond/Cly.hpp"
#include "GhostSecond/Pink.hpp"
#include "GhostSecond/Ink.hpp"


bool SecondScene::DebugMode = false;
const std::vector<Engine::Point> SecondScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int SecondScene::MapWidth = 30, SecondScene::MapHeight = 30;
const int SecondScene::BlockSize = 64;
const float SecondScene::DangerTime = 7.61;
const Engine::Point SecondScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point SecondScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);

Engine::Point SecondScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}



ALLEGRO_BITMAP* SecondScene::frightenedBitmap = nullptr;

void SecondScene::Initialize() {
    std::srand(std::time(nullptr)); 
    opening = true;
    openingTimer = 0;
    startSound = AudioHelper::PlaySample("Pacman/start-game.wav", false, AudioHelper::SFXVolume);
    
    WinTriggered = false;
    mapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 3;
    money = 0;
    SpeedMult = 1;
    total_dot = 0;

    /*if (!frightenedBitmap)
        frightenedBitmap = al_load_bitmap("Resource/images/ghost/ghost_frighten.png");*/
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(DotsGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(EffectGroup = new Group());

    ReadMap();
    ConstructUI();

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    playerOneLabel = new Engine::Label("PLAYER ONE", "prstartk.ttf", 64, w / 2-150, h / 2 - 80, 255, 255, 0, 255, 0.5, 0.5);
    readyLabel     = new Engine::Label("READY!",     "prstartk.ttf", 64, w / 2-150, h / 2 + 30, 0, 255, 255, 255, 0.5, 0.5);

    UIGroup->AddNewObject(playerOneLabel);
    UIGroup->AddNewObject(readyLabel);

    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    
    UIGroup->AddNewObject(imgTarget);
    keyPressed.clear();
    slot_yet=1;
    //timer
    remainingTime = 60.0f; // 從 60 秒開始

    // 建立 Timer Label
    timerLabel = new Engine::Label("1:00", "prstartk.ttf", 28, 1294, 128, 255, 255, 255, 255);
    UIGroup->AddNewObject(timerLabel);
    
}




void SecondScene::Terminate() {
    //AudioHelper::StopBGM(bgmId);
    //AudioHelper::StopSample(deathBGMInstance);
    //deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    
    
    IScene::Terminate();
}
void SecondScene::Update(float deltaTime) {
    if (paused) return;
    
    if (!WinTriggered && !opening) {
        remainingTime -= deltaTime;
        if (remainingTime <= 0) {
            remainingTime = 0;
            WinTriggered = true;
        }

        int seconds = static_cast<int>(remainingTime);
        int minutes = seconds / 60;
        seconds = seconds % 60;

        char buffer[8];
        std::snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
        timerLabel->Text = buffer;
    }
        
    
    if(!opening){
        money = player->money;
        UIMoney->Text = std::string("$") + std::to_string(money);
        if(player){
            if(player->power_mode){
                for(int i=0; i<4; i++){
                    ghost[i]->frighten=true;
                    ghost[i]->frightenedTimer=0;
                    ghost[i]->Speed=80;
                }
                player->power_mode=false;
            }
            
            if(player->pause){
                for(int i=0; i<4; i++)
                    ghost[i]->pause=true;
                freeze_coldown=0;
                player->pause=false;
                freeze_mode=true;
            }
            if(freeze_mode&&freeze_coldown>100){
                for(int i=0; i<4; i++)
                    ghost[i]->pause=false;
            }
            if(freeze_mode)
                freeze_coldown++;

            for(int i=0; i<4; i++){
                if(ghost[i]){
                    if(ghost[i]->caught==true){
                        ghost[i]->caught=false;
                        lives--;
                        player->get_hit=true;
                        red_coldown=0;
                }
            
            }
        }
        if(red_coldown < 10)
            red_coldown++;
        if(red_coldown==7)
            player->get_hit=false;
        
        UILives ->Text =std::string("Life ") + std::to_string(lives);
        if(lives == 0) {
            Engine::LOG(Engine::INFO) << "Game Over, switching to game-over scene.";
            Engine::GameEngine::GetInstance().ChangeScene("lose_second");
            return;
        }
    }

    }
    

    if (opening) {
        openingTimer += deltaTime;
        if (openingTimer >= 2.0f) {
            opening = false;

            // 移除文字
            if (playerOneLabel)
                UIGroup->RemoveObject(playerOneLabel->GetObjectIterator());
            if (readyLabel)
                UIGroup->RemoveObject(readyLabel->GetObjectIterator());
            player = new Pac(10 * BlockSize + BlockSize / 2, 7 * BlockSize + BlockSize / 2 - 64);
            ghost[0]=new Blin(1 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2);
            ghost[1]=new Pink(1 * BlockSize + BlockSize / 2, 18 * BlockSize + BlockSize / 2);
            ghost[2]=new Ink(11 * BlockSize + BlockSize / 2, 16 * BlockSize + BlockSize / 2);
            ghost[3]=new Cly(11 * BlockSize + BlockSize / 2, 13 * BlockSize + BlockSize / 2);
        } else {
            return;
        }
    }
    
    if (player) {
        player->Update(deltaTime);
        // 攝影機跟隨玩家
        Engine::Point playerPos = player->GetPosition();
        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

        cameraPos.x = playerPos.x - screenW / 2+ 2 * BlockSize;;
        cameraPos.y = playerPos.y - screenH / 2;
        GhostSecond::CameraPos = cameraPos;
        // 限制攝影機不超出地圖邊界
        // 限制攝影機不超出地圖邊界（只在地圖大於畫面時才限制）
        // 否則自動置中地圖
        int gameScreenW = 64*20; // 左邊地圖的畫面寬度

        // 攝影機限制在地圖內，但只考慮左邊地圖畫面寬度
        if (MapWidth * BlockSize > gameScreenW)
            cameraPos.x = std::max(0.0f, std::min<float>(cameraPos.x, MapWidth * BlockSize - gameScreenW));
        else
            cameraPos.x = (MapWidth * BlockSize - gameScreenW) / 2;

        if (MapHeight * BlockSize > screenH)
            cameraPos.y = std::max(0.0f, std::min<float>(cameraPos.y, MapHeight * BlockSize - screenH));
        else
            cameraPos.y = (MapHeight * BlockSize - screenH) / 2;

    

        int dx = 0, dy = 0;
        if (keyPressed.count(ALLEGRO_KEY_UP)) {
                dx = 0; dy = -1;
        } else if (keyPressed.count(ALLEGRO_KEY_DOWN)) {
                dx = 0; dy = 1;
        } else if (keyPressed.count(ALLEGRO_KEY_LEFT)) {
                dx = -1; dy = 0;
        } else if (keyPressed.count(ALLEGRO_KEY_RIGHT)) {
                dx = 1; dy = 0;
        }

        if (dx != 0 || dy != 0) {
        // 取得目前在地圖上的格子座標
            int gridX = static_cast<int>(player->GetPosition().y) / SecondScene::BlockSize;
            int gridY = static_cast<int>(player->GetPosition().x) / SecondScene::BlockSize;

            int targetX = gridX + dy;
            int targetY = gridY + dx;

            // 檢查是否在邊界內，且不是牆壁
            if (/*targetX >= 0 && targetX < SecondScene::MapHeight &&
                targetY >= 0 && targetY < SecondScene::MapWidth &&*/                    map_dot[targetX][targetY] != -1) {
                player->MoveDirection(dx, dy);
                map_dot[targetX][targetY]=0;
                UpdateMiniMapCell(targetX, targetY);
            }
        }
    }

    DotsGroup->Update(deltaTime);
    

    if (WinTriggered) {
        Engine::LOG(Engine::INFO) << "WinTriggered = true, switching to win-scene.";
        Engine::GameEngine::GetInstance().ChangeScene("win_second");
        return;
    }
    if (replayRequested) {
        replayRequested = false;
        Engine::GameEngine::GetInstance().ChangeScene("second");
        return;
    }
    for(int i=0; i<4; i++)
    {
        if(ghost[i]){
        ghost[i]->setPacmanPos(player->Position);
        ghost[i]->Update(deltaTime);
    }
    }
    

    
}
void SecondScene::Draw() const {
    IScene::Draw();
     // 套用攝影機偏移
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, -cameraPos.x, -cameraPos.y);
    al_use_transform(&transform);

    if (TileMapGroup) TileMapGroup->Draw();
    if (DotsGroup) DotsGroup->Draw();
    if (player) player->Draw();

    for(int i=0; i<4; i++){
        if(ghost[i]){
        
        ghost[i]->Draw();
    }
    }
    
    // 恢復 transform 為預設，繪製 UI (不受攝影機影響)
    al_identity_transform(&transform);
    al_use_transform(&transform);

    if (UIGroup) UIGroup->Draw();


    

    if (miniMapBitmap&&!opening) {
    float scale = 1.0;
    int x = 1280 + 80; // 根據你右側 UI 位置微調
    int y = 640;       // 底部空白處
    al_draw_scaled_bitmap(miniMapBitmap, 0, 0, MiniMapSize, MiniMapSize,
                          x, y, MiniMapSize * scale, MiniMapSize * scale, 0);

    // 畫出玩家在小地圖的位置（紅色方塊）
    float cellSize = MiniMapSize / static_cast<float>(MapWidth);
    Engine::Point pos = player->GetPosition();
    int gridX = pos.x / BlockSize;
    int gridY = pos.y / BlockSize;

    al_draw_filled_rectangle(x + gridX * cellSize, y + gridY * cellSize,
                             x + (gridX + 1) * cellSize, y + (gridY + 1) * cellSize,
                             al_map_rgb(255, 0, 0));
}
    
}



void SecondScene::ReadMap() {
    //std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    std::string filename = std::string("Resource/testing.txt");
    // Read map file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
    
    while (fin >> c) {
        switch (c) {
            case '0': 
                mapData.push_back(0); 
                break;
            case '1': 
                mapData.push_back(1); 
                
                break;
            case '2': 
                mapData.push_back(2); 
                
                break;  // For testing, treat '2' as '1'.
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            if(num==0||num==1)
            {
                
                if(num==1){
                    map_dot[i][j]=1;
                    total_dot++;
                }
                else if(num==0)
                {
                    map_dot[i][j]=0;
                }
                    
            }
            else if(num==2)
            {
                
                map_dot[i][j]=-1;
            }
            
            //mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num==2)
                TileMapGroup->AddNewObject(new Engine::Image("play/wall_test.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/floor_2.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
    Dot* dot;
    
    for(int i=0; i<14; i++)
    {
        random[i]=rand()%total_dot;
        
        if(i>0)
        {
            for(int j=0; j<i; j++)
            {
                if(random[i]==random[j]){
                    i--;
                    break;
                }
            }
            
        }
        
    }

    int powerRandom[5];
    for (int i = 0; i < 5; i++) {
        powerRandom[i] = rand() % total_dot;
        for (int j = 0; j < i; j++) {
            if (powerRandom[i] == powerRandom[j] || 
                powerRandom[i] == random[0] || 
                powerRandom[i] == random[1] || 
                powerRandom[i] == random[2]) {
                i--; // 重新挑一個
                break;
            }
        }
    }



    int k=0;
    for(int i=0; i<30; i++)
    {
        for(int j=0; j<30; j++)
        {
            
            
            if(map_dot[i][j] == 1)
            {
                bool isPower = false;
                for (int p = 0; p < 5; p++) {
                    if (k == powerRandom[p]) {
                        isPower = true;
                        break;
                    }
                }

                if (isPower) {
                    DotsGroup->AddNewObject(dot = new PD(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                    map_dot[i][j]=2;
                }
                else if(k==random[0]||k==random[1]||k==random[2]){
                    DotsGroup->AddNewObject(dot = new Star(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                    map_dot[i][j]=2;//item
                }
                else if(k==random[3]||k==random[4]||k==random[5]){
                    DotsGroup->AddNewObject(dot = new Speed(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                    map_dot[i][j]=2;
                }
                else if(k==random[6]||k==random[7]||k==random[8]){
                    DotsGroup->AddNewObject(dot = new Ice(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                    map_dot[i][j]=2;
                }
                else if(k==random[9]||k==random[10]||k==random[11]||k==random[12]||k==random[13]){
                    DotsGroup->AddNewObject(dot = new Scroll(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                    map_dot[i][j]=2;
                }
                else{
                    DotsGroup->AddNewObject(dot = new NormalDot(j * BlockSize + BlockSize / 2, i * BlockSize + BlockSize / 2));
                }

                
                k++;
            }
            
        }   
    }
    
    GenerateMiniMap();

}

void SecondScene::ConstructUI() {
     UIGroup = new Group();
    AddNewObject(UIGroup);
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/floor.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage 2") , "prstartk.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "prstartk.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "prstartk.ttf", 24, 1294, 88));
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    pauseLabel = new Engine::Label("PAUSED", "prstartk.ttf", 72, w / 2-150, h / 2, 255, 255, 255, 255, 0.5, 0.5);
    pauseLabel->Visible = false;
    UIGroup->AddNewObject(pauseLabel);

    
    Engine::ImageButton* replayBtn = new Engine::ImageButton(
        "play/replay_button.png", "play/replay_button_hover.png", 
        1450, 20, 130, 140
    );
    replayBtn->SetOnClickCallback(std::bind(&SecondScene::ReplayOnClick, this));
    UIGroup->AddNewControlObject(replayBtn);

}


void SecondScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    keyPressed.insert(keyCode);  // 記下按下的鍵

    if (!opening && keyCode == ALLEGRO_KEY_SPACE) {
        paused = !paused;
        if (pauseLabel)
            pauseLabel->Visible = paused;
    }
}

void SecondScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);    
    keyPressed.erase(keyCode);   // 移除放開的鍵
}


void SecondScene::GenerateMiniMap() {
    if (miniMapBitmap) {
        al_destroy_bitmap(miniMapBitmap);
    }
    miniMapBitmap = al_create_bitmap(MiniMapSize, MiniMapSize);
    ALLEGRO_BITMAP* oldTarget = al_get_target_bitmap();
    al_set_target_bitmap(miniMapBitmap);
    al_clear_to_color(al_map_rgb(0, 0, 0)); // 背景黑

    float cellSize = MiniMapSize / static_cast<float>(MapWidth);

    for (int i = 0; i < MapHeight; ++i) {
        for (int j = 0; j < MapWidth; ++j) {
            ALLEGRO_COLOR color;
            if (map_dot[i][j] == 0)
                color = al_map_rgb(128, 138, 135); 
            else if (map_dot[i][j] == 1)
                color = al_map_rgb(230, 184, 0);   // dot - 黃色
            else if (map_dot[i][j] == 2)//item
                color = al_map_rgb(64, 224, 208); 
            else
                color = al_map_rgb(20, 20, 20);    

            al_draw_filled_rectangle(j * cellSize, i * cellSize,
                                     (j + 1) * cellSize, (i + 1) * cellSize,
                                     color);
        }
    }

    al_set_target_bitmap(oldTarget);
}


void SecondScene::UpdateMiniMapCell(int i, int j) {
    if (!miniMapBitmap) return;

    ALLEGRO_BITMAP* oldTarget = al_get_target_bitmap();
    al_set_target_bitmap(miniMapBitmap);

    float cellSize = MiniMapSize / static_cast<float>(MapWidth);
    ALLEGRO_COLOR color;

    if (map_dot[i][j] == 0)
        color = al_map_rgb(128, 138, 135); 
    else if (map_dot[i][j] == 1)
        color = al_map_rgb(230, 184, 0);   // dot - 黃色
    else if (map_dot[i][j] == 2)//item
        color = al_map_rgb(64, 224, 208); 
    else
        color = al_map_rgb(20, 20, 20); 
    al_draw_filled_rectangle(j * cellSize, i * cellSize,
                             (j + 1) * cellSize, (i + 1) * cellSize,
                             color);

    al_set_target_bitmap(oldTarget);
}
void SecondScene::ReplayOnClick() {
    replayRequested = true;
}

void SecondScene::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
    if (UIGroup)
        UIGroup->OnMouseDown(button, mx, my);
}

void SecondScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    if (UIGroup)
        UIGroup->OnMouseMove(mx, my);
}

void SecondScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (UIGroup)
        UIGroup->OnMouseUp(button, mx, my);
}