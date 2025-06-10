#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>


#include "UI/Component/ImageButton.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"

#include "Engine/IScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Component/Label.hpp"
#include "Dot/Dot.hpp"
#include "Pacman/Pacman.hpp"
#include "Dot/NormalDot.hpp"
#include "Ghost/Blinky.hpp"
#include "Ghost/Inky.hpp"
#include "Ghost/Pinky.hpp"
#include "Ghost/Clyde.hpp"

// TODO HACKATHON-4 (1/3): Trace how the game handles keyboard input.
// TODO HACKATHON-4 (2/3): Find the cheat code sequence in this file.
// TODO HACKATHON-4 (3/3): When the cheat code is entered, a plane should be spawned and added to the scene.
// TODO HACKATHON-5 (1/4): There's a bug in this file, which crashes the game when you win. Try to find it.
// TODO HACKATHON-5 (2/4): The "LIFE" label are not updated when you lose a life. Try to fix it.

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {
    ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
    ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEY_MODIFIERS, ALLEGRO_KEY_ENTER
};
Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

std::vector<Ghost*> ghostList;

void PlayScene::Initialize() {
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
    /*
    // Add four ghosts in the corners
    AddNewObject(new Blinky(1 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2)); // top-left
    AddNewObject(new Pinky(18 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2)); // top-right
    AddNewObject(new Inky(1 * BlockSize + BlockSize / 2, 11 * BlockSize + BlockSize / 2));  // bottom-left
    AddNewObject(new Clyde(18 * BlockSize + BlockSize / 2, 11 * BlockSize + BlockSize / 2)); // bottom-right
    for (auto g : ghostList) AddNewObject(g);*/
}




void PlayScene::Terminate() {
    //AudioHelper::StopBGM(bgmId);
    //AudioHelper::StopSample(deathBGMInstance);
    //deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    
    IScene::Terminate();
    
}
void PlayScene::Update(float deltaTime) {
    if (paused) return;

    

    if (opening) {
        openingTimer += deltaTime;
        if (openingTimer >= 2.0f) {
            opening = false;

            // 移除文字
            if (playerOneLabel)
                UIGroup->RemoveObject(playerOneLabel->GetObjectIterator());
            if (readyLabel)
                UIGroup->RemoveObject(readyLabel->GetObjectIterator());
            player = new Pacman(10 * BlockSize + BlockSize / 2, 6 * BlockSize + BlockSize / 2 - 64);
        } else {
            return;
        }
    }
    
    if (player) {
        player->Update(deltaTime);
    }
    //ghost
    /*if(ghost_1){
        ghost_1->setPacmanPos(player->GetPosition());
        ghost_1->Update(deltaTime);
    }
    if(ghost_2){
        ghost_2->setPacmanPos(player->GetPosition());
        ghost_2->Update(deltaTime);
    }
    if(ghost_3){
        ghost_3->setPacmanPos(player->GetPosition());
        ghost_3->Update(deltaTime);
    }
    if(ghost_4){
        ghost_4->setPacmanPos(player->GetPosition());
        ghost_4->Update(deltaTime);
    }*/
    for(int i=0; i<4; i++){
        if(ghost[i]){
            ghost[i]->setPacmanPos(player->GetPosition());
            ghost[i]->Update(deltaTime);
        }
    }

    for(int i=0; i<4; i++){
        if(ghost[i]){
            if(ghost[i]->caughtPacman==true){
                ghost[i]->caughtPacman=false;
                lives--;
            }
            
        }
    }
    UILives ->Text =std::string("Life ") + std::to_string(lives);
    if(lives == 0) {
        Engine::LOG(Engine::INFO) << "Game Over, switching to game-over scene.";
        Engine::GameEngine::GetInstance().ChangeScene("lose");
        return;
    }
    //ghost

    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
        money = player->money; 

        if(player->dotsEaten==total_dot){
            WinTriggered=true;
        }

        UIMoney->Text = std::string("$") + std::to_string(money);

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
            int gridX = static_cast<int>(player->GetPosition().y) / PlayScene::BlockSize;
            int gridY = static_cast<int>(player->GetPosition().x) / PlayScene::BlockSize;

            int targetX = gridX + dy;
            int targetY = gridY + dx;

            // 檢查是否在邊界內，且不是牆壁
            if (targetX >= 0 && targetX < PlayScene::MapHeight &&
                targetY >= 0 && targetY < PlayScene::MapWidth &&
                map_dot[targetX][targetY] != -1) {
                player->MoveDirection(dx, dy);
            }
        }
    

    DotsGroup->Update(deltaTime);
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;

    if (WinTriggered) {
        Engine::LOG(Engine::INFO) << "WinTriggered = true, switching to win-scene.";
        Engine::GameEngine::GetInstance().ChangeScene("win-scene");
        return;
    }

    

    if(slot_yet&&!opening)
    {
        //slot
        slot_yet=0;
        Engine::ImageButton *btn;
        btn = new Engine::ImageButton("play/floor_test.png", "stage-select/bott.png", 1350, 700, 185, 80);
        btn->SetOnClickCallback(std::bind(&PlayScene::SlotOnClick, this, 1));
        AddNewControlObject(btn);   // 這樣 button 可以點
        AddNewObject(new Engine::Label("10 Dots!", "prstartk.ttf", 20, 1450, 740, 0, 0, 0, 255, 0.5, 0.5));

        slotMachine = new SlotMachine(1345, 600);
        
        //ghost
        ghost[0]= new Blinky(1 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2);
        ghost[1]= new Pinky(18 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2);
        ghost[2]= new Inky(1 * BlockSize + BlockSize / 2, 11 * BlockSize + BlockSize / 2);
        ghost[3]= new Clyde(18 * BlockSize + BlockSize / 2, 11 * BlockSize + BlockSize / 2);
        
        //ghost
     
    //slot
    }

    //slot
    if(slot_mode&&slotMachine)
        slotMachine->Update(deltaTime);
    if(slot_mode&&slotMachine->allStopped)
    {
        if(slotMachine->jackpot){
            player->money+=1000;
            //UIMoney->Text = std::string("$") + std::to_string(money);
            slotMachine->jackpot=false;
        }
        slot_mode=false;
    }
   
}
void PlayScene::Draw() const {
    IScene::Draw();
    //slot
    
    
    if(!opening&&slotMachine)
        slotMachine->Draw();
    
    if(!opening) {
        for(int i=0; i<4; i++){
            ghost[i]->Draw();
        }
       
    }
    
    if (!opening && player) {
        player->Draw(); // 開場時不畫 pacman
    }

    if (DebugMode) {
        // Draw reverse BFS distance on all reachable blocks.
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    // Not elegant nor efficient, but it's quite enough for debugging.
                    Engine::Label label(std::to_string(mapDistance[i][j]), "prstartk.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                }
            }
        }
    }
}



void PlayScene::ReadMap() {
    //std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    std::string filename = std::string("Resource/test_map.txt");
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
    for(int i=0; i<13; i++)
    {
        for(int j=0; j<20; j++)
        {
            if(map_dot[i][j] == 1)
            {
                DotsGroup->AddNewObject(dot=new NormalDot(j*BlockSize+BlockSize/2, i*BlockSize+BlockSize/2));
            }
            
        }   
    }

}

void PlayScene::ConstructUI() {
     UIGroup = new Group();
    AddNewObject(UIGroup);
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/floor.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "prstartk.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "prstartk.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "prstartk.ttf", 24, 1294, 88));
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    pauseLabel = new Engine::Label("PAUSED", "prstartk.ttf", 72, w / 2-150, h / 2, 255, 255, 255, 255, 0.5, 0.5);
    pauseLabel->Visible = false;
    UIGroup->AddNewObject(pauseLabel);



}


void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    keyPressed.insert(keyCode);  // 記下按下的鍵

    if (!opening && keyCode == ALLEGRO_KEY_SPACE) {
        paused = !paused;
        if (pauseLabel)
            pauseLabel->Visible = paused;
    }
}

void PlayScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);    
    keyPressed.erase(keyCode);   // 移除放開的鍵
}
//slot
void PlayScene::SlotOnClick(int stage){
    if(!slot_mode){
        if(player->money>=10){
            player->money-=10;
            slotMachine->StartSpin();
            slot_mode= true;
        } 
    }
    
    
}