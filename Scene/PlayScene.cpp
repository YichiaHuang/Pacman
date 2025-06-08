#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>



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
void PlayScene::Initialize() {
    WinTriggered = false;
    mapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 10;
    money = 0;
    SpeedMult = 1;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());

    AddNewObject(DotsGroup = new Group());

    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    ReadMap();
    //ReadEnemyWave();
    
    //mapDistance = CalculateBFSDistance();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    /*// Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    bgmId = AudioHelper::PlayBGM("play.ogg");*/
    
    player = new Pacman(1 * BlockSize + BlockSize / 2, 1 * BlockSize + BlockSize / 2);
    AddNewObject(player);
    
}
void PlayScene::Terminate() {
    //AudioHelper::StopBGM(bgmId);
    //AudioHelper::StopSample(deathBGMInstance);
    //deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
    
}
void PlayScene::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
        money = player->dotsEaten; // 每吃一個點數增加10

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
            int gridX = static_cast<int>(player->Position.y) / PlayScene::BlockSize;
            int gridY = static_cast<int>(player->Position.x) / PlayScene::BlockSize;

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
    player->Update(deltaTime);
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;

    if (WinTriggered) {
        Engine::LOG(Engine::INFO) << "WinTriggered = true, switching to win-scene.";
        Engine::GameEngine::GetInstance().ChangeScene("win-scene");
    return;
    }
   
}
void PlayScene::Draw() const {
    IScene::Draw();
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
                map_dot[i][j]=0;
                if(num==1)
                    map_dot[i][j]=1;
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
            //std::cout<<map_dot[i][j]<<" ";
            if(map_dot[i][j] == 1)
            {
                DotsGroup->AddNewObject(dot=new Dot(j*BlockSize+BlockSize/2, i*BlockSize+BlockSize/2));
            }
            
        }   
        //std::cout<<std::endl;
    }

}

void PlayScene::ConstructUI() {
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/floor.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "prstartk.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "prstartk.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "prstartk.ttf", 24, 1294, 88));
    
}


void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    keyPressed.insert(keyCode);  // 記下按下的鍵
}

void PlayScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);    // 注意這邊呼叫的是 OnKeyUp，不是 OnKeyDown
    keyPressed.erase(keyCode);   // 移除放開的鍵
}
