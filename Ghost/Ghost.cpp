#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include <queue>

#include "Ghost.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"

using namespace std;
using namespace Engine;
//Engine::Point GhostSecond::CameraPos;
int bfs(Engine::Point A, Engine::Point B);

Ghost::Ghost(float x, float y)
    : x(x), y(y), Position(x, y), gridX(x / PlayScene::BlockSize),
     gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0), Speed(100) {
    originX=x / PlayScene::BlockSize;
    originY=y / PlayScene::BlockSize;
    origin=Engine::Point(originX, originY);
     }

Ghost::~Ghost() {
    if (spriteSheet) {
        al_destroy_bitmap(spriteSheet);
        spriteSheet = nullptr;
    }
}


int Ghost::bfs(Engine::Point A, Engine::Point B) {
    
    
    
    
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    int W = 20;//PlayScene::MapWidth;
    int H = 13;//PlayScene::MapHeight;
    int vis[25][20] = {};
    int dist[25][20] = {};

    int ax = static_cast<int>(A.x);
    int ay = static_cast<int>(A.y);
    int bx = static_cast<int>(B.x);
    int by = static_cast<int>(B.y);

    if (ax < 0 || ax >= W || ay < 0 || ay >= H || bx < 0 || bx >= W || by < 0 || by >= H)
        return INT_MAX;

    dist[ay][ax] = 0;
    std::queue<std::pair<int, int>> q;
    q.emplace(ax, ay);

    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [cx, cy] = q.front(); q.pop();
        if (vis[cy][cx]) continue;
        vis[cy][cx] = 1;

        if (cx == bx && cy == by)
            return dist[cy][cx];

        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
            //nx=(nx+30)%20;
            //ny=(ny+30)%13;

            if (scene.map_dot[ny][nx] == -1) continue;
            if (!vis[ny][nx]) {
                dist[ny][nx] = dist[cy][cx] + 1;
                q.emplace(nx, ny);
            }
        }
    }
    return INT_MAX;
}
void Ghost::setDir() {
    Engine::Point predictTargetPos = pacmanPos;
    


    if (predict_mode && !frighten&&!return_mode) {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    // 每幀都根據 Pacman 方向預測前方幾格的位置
    int predictSteps = 2; // 預測距離
    //int offset = rand() % 3 - 1; // -1, 0, or 1，讓預測目標微偏移，避免重複走法
        int offset=0;
    int pacX = static_cast<int>(scene.player->Position.x) / PlayScene::BlockSize;
    int pacY = static_cast<int>(scene.player->Position.y) / PlayScene::BlockSize;

    int dirX = scene.player->moveDirX;
    int dirY = scene.player->moveDirY;

    int targetX = pacX + dirX * predictSteps + offset;
    int targetY = pacY + dirY * predictSteps + offset;

    // 邊界檢查
    targetX = std::max(0, std::min(targetX, PlayScene::MapWidth - 1));
    targetY = std::max(0, std::min(targetY, PlayScene::MapHeight - 1));

    // 如果該位置是牆，就 fallback 回 pacman 目前位置
    if (scene.map_dot[targetY][targetX] == -1)
        predictTargetPos = Engine::Point(pacX, pacY);
    else
        predictTargetPos = Engine::Point(targetX, targetY);
}

    
    
    
    
    
    
    
    vector<Engine::Point> nbrs;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    Engine::Point prevPos(moveDirX, moveDirY);
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dxs[i];
        int ny = gridY + dys[i];
        if (nx < 0 || nx >= PlayScene::MapWidth || ny < 0 || ny >= PlayScene::MapHeight)
            continue;
        //nx=(nx+30)%20;
        //ny=(ny+30)%13;
        
        if (scene.map_dot[ny][nx] != -1&&!(f_firststep||first_step)) {
            if (!(dxs[i] == -prevPos.x && dys[i] == -prevPos.y)) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    if (nbrs.empty()) {
        for (int i = 0; i < 4; i++) {
            int nx = gridX + dxs[i];
            int ny = gridY + dys[i];
            if (nx < 0 || nx >= PlayScene::MapWidth || ny < 0 || ny >= PlayScene::MapHeight)
                continue;
            //nx=(nx+30)%20;
            //ny=(ny+30)%13;

            if (scene.map_dot[ny][nx] != -1) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
            if(f_firststep)
                f_firststep=false;
            if(first_step)
                first_step=false;
        }
    }
    Engine::Point chosen(0, 0);

    if(return_mode){
        int nowDis = INT_MAX;
    
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d;
        if(return_mode)
            d=bfs(nextPos, origin);
        
        if (d < nowDis) {
            nowDis = d;
            chosen = c;
        }
    }
    }
    else if(!(frighten||flee)){
    int nowDis = INT_MAX;
    
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d;
        
        if(!predict_mode)
            d = bfs(nextPos, pacmanPos);
        else if(predict_mode)
            d = bfs(nextPos, predictTargetPos);  // pacmanPos 必須已經是 grid 座標
        
        if (d < nowDis) {
            nowDis = d;
            chosen = c;
        }
    }

    if(nowDis>20||(random_mode&&!frighten)){
        int size=nbrs.size();
        int r=rand();
        r=r%size;
        int i=0;
        for (auto& c : nbrs) {
            i++;
            if(r==i){
                chosen = c;
            }
        }
    }
    }
    else if(frighten||flee){
        int nowDis = -1;
    
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d = bfs(nextPos, pacmanPos);  // pacmanPos 必須已經是 grid 座標
        if (d > nowDis) {
            nowDis = d;
            chosen = c;
        }
    }
    }

    moveDirX = chosen.x;
    moveDirY = chosen.y;
    if(return_mode){
        if (moveDirX == 1 && moveDirY == 0) {
        faceDir = RIGHT; tickCount_y = 1;tickCount_x = 0;
    } else if (moveDirX == -1 && moveDirY == 0) {
        faceDir = LEFT; tickCount_y = 0;tickCount_x = 1;
    } else if (moveDirX == 0 && moveDirY == 1) {
        faceDir = DOWN; tickCount_y = 0;tickCount_x = 0;
    } else if (moveDirX == 0 && moveDirY == -1) {
        faceDir = UP; tickCount_y = 1;tickCount_x = 1;
    }
    }
    else{
    if (moveDirX == 1 && moveDirY == 0) {
        faceDir = RIGHT; tickCount_y = 2;
    } else if (moveDirX == -1 && moveDirY == 0) {
        faceDir = LEFT; tickCount_y = 1;
    } else if (moveDirX == 0 && moveDirY == 1) {
        faceDir = DOWN; tickCount_y = 0;
    } else if (moveDirX == 0 && moveDirY == -1) {
        faceDir = UP; tickCount_y = 3;
    }
    }
}



void Ghost::Update(float deltaTime) {
    
    
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    
    float pacX = scene.player->Position.x;
    float pacY = scene.player->Position.y;
    float distToPacman = std::hypot(Position.x - pacX, Position.y - pacY);

    if (distToPacman < 16.0f && cold > 50 ) {
        if(!frighten){
        caught = true;
        cold = 0;
        }
        else if(frighten&&!return_mode){
            //Reset();
            return_pre=true;
            //Speed=300;
            scene.player->money+=200;
            cold = 0;
        }
    }
    cold++;
    

    if(frighten){
        frightenedTimer++;
    }
    if(frighten&&frightenedTimer>300&&!return_mode&&!return_pre){
        frighten=false;
        Speed=100;
        first_step=true;
    }


    if (return_mode && /*Position.x==x && Position.y==y*/gridX==originX&&gridY==originY) {
        return_mode = false;
        Speed = 100;
        Position = Engine::Point(x, y);
        frighten=false;
        first_step=true;
    }



    if(pause)
        return;

    float centerX = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float centerY = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float distToCenter = std::hypot(Position.x - centerX, Position.y - centerY);

    if (distToCenter < 1.5f) {
        Position.x = centerX;
        Position.y = centerY;
        if(return_pre){
            return_pre=false;
            return_mode=true;
            Speed=300;
            first_step=true;
        }
        setDir();
        gridX += moveDirX;
        gridY += moveDirY;

        //gridX=(gridX+30)%30;
        //gridY=(gridY+30)%30;
    }

    Position.x += moveDirX * Speed * deltaTime;
    Position.y += moveDirY * Speed * deltaTime;

    if (Position.x < 0) Position.x = PlayScene::MapWidth * PlayScene::BlockSize-PlayScene::BlockSize/2;
    if (Position.y < 0) Position.y = PlayScene::MapHeight * PlayScene::BlockSize-PlayScene::BlockSize/2;
    if (Position.x >= PlayScene::MapWidth * PlayScene::BlockSize) 
        Position.x = PlayScene::BlockSize/2;
    if (Position.y >= PlayScene::MapHeight * PlayScene::BlockSize)
        Position.y = PlayScene::BlockSize/2;
    if(!return_mode){
    tick++;
    if(tick >= 10) {
        tick = 0;
        tickCount_x = (tickCount_x + 1) % 2;
    }
    }
    
    
}
void Ghost::setPacmanPos(const Engine::Point &pos) {
    // 傳入前請確保 pacmanPos 是格子座標
    
    
    pacmanPos = Engine::Point(
        static_cast<int>(pos.x) / PlayScene::BlockSize,
        static_cast<int>(pos.y) / PlayScene::BlockSize
    );
    
}


void Ghost::Draw() const {
    
    if (!spriteSheet) return; // 確保有精靈圖
    
    float cx = frameW / 2.0f;
    float cy = frameH / 2.0f;
    
    // 計算屏幕坐標（世界坐標減去攝影機偏移）
    float screenX = Position.x;//+ CameraPos.x/2;
    float screenY = Position.y; //+ CameraPos.y/2;
    
    // 檢查是否在可見範圍內（可選優化）
    int screenWidth = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int screenHeight = Engine::GameEngine::GetInstance().GetScreenSize().y;
    
    if (screenX  < 0 ||
        screenY  < 0 ) {
        return; // 如果不在屏幕內就不繪製
    }
    if(!frighten){
    al_draw_tinted_scaled_rotated_bitmap_region(
        spriteSheet,
        tickCount_x * frameW, tickCount_y * frameH, frameW, frameH,
        al_map_rgba(255, 255, 255, 255), // 無染色
        cx, cy,                          // 旋轉中心
        screenX, screenY,                // 屏幕位置
        3.0f, 3.0f,                      // 縮放
        0,                               // 旋轉角度
        0                                // 標誌
    );
    }
    else if(return_mode){
        al_draw_tinted_scaled_rotated_bitmap_region(
        ScareSheet,
        tickCount_x * frameW, tickCount_y * frameH, frameW, frameH,
        al_map_rgba(255, 255, 255, 255), // 無染色
        cx, cy,                          // 旋轉中心
        screenX, screenY,                // 屏幕位置
        3.0f, 3.0f,                      // 縮放
        0,                               // 旋轉角度
        0                                // 標誌
    );
    }
    else if(frighten){
        al_draw_tinted_scaled_rotated_bitmap_region(
        FrightenSheet,
        tickCount_x * frameW, 0, frameW, frameH,
        al_map_rgba(255, 255, 255, 255), // 無染色
        cx, cy,                          // 旋轉中心
        screenX, screenY,                // 屏幕位置
        3.0f, 3.0f,                      // 縮放
        0,                               // 旋轉角度
        0                                // 標誌
    );
    }
}

void Ghost::Reset() {
    /*Position = Engine::Point(x, y); // 原始建構座標
    gridX = x / PlayScene::BlockSize;
    gridY = y / PlayScene::BlockSize;*/
    //originX=x / PlayScene::BlockSize;
    //originY=y / PlayScene::BlockSize;
    //origin=Engine::Point(originX, originY);
    //return_mode=true;
    //return_pre=true;
    //moveDirX = moveDirY = 0;
    //Speed = 300;
    //frightenedTimer = 0;
    //Speed = 100;
    //spriteSheet = normalSprite;
    //caught = false;
    //cold = 0;
    //first_step=true;
}