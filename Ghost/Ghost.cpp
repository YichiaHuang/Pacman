/*#include <allegro5/allegro_primitives.h>
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

int bfs(Engine::Point A, Engine::Point B);

Ghost::Ghost(float x, float y)
    : x(x), y(y), Position(x, y), gridX(x / PlayScene::BlockSize),
     gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0), Speed(100) {}

Ghost::~Ghost() {
    if (spriteSheet && spriteSheet != normalSprite) {
        al_destroy_bitmap(spriteSheet);
        spriteSheet = nullptr;
    }
}

int bfs(Engine::Point A, Engine::Point B) {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    int W = 20;//PlayScene::MapWidth;
    int H = 13;//PlayScene::MapHeight;
    int vis[13][20] = {};
    int dist[13][20] = {};

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
            nx = (nx + 20) % 20;
            ny = (ny + 13) % 13;
            if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
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
    std::vector<Engine::Point> nbrs;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    Engine::Point prevPos(moveDirX, moveDirY);
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dxs[i];
        int ny = gridY + dys[i];
        if (nx >= 0 && nx < PlayScene::MapWidth &&
            ny >= 0 && ny < PlayScene::MapHeight &&
            scene.map_dot[ny][nx] != -1 &&
            !(dxs[i] == -prevPos.x && dys[i] == -prevPos.y)) {
            nbrs.emplace_back(dxs[i], dys[i]);
        }
    }

    if (nbrs.empty()) {
        for (int i = 0; i < 4; i++) {
            int nx = gridX + dxs[i];
            int ny = gridY + dys[i];
            if (nx >= 0 && nx < PlayScene::MapWidth &&
                ny >= 0 && ny < PlayScene::MapHeight &&
                scene.map_dot[ny][nx] != -1) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    int nowDis = INT_MAX;
    Engine::Point chosen(0, 0);
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d = bfs(nextPos, targetPos);
        if (d < nowDis) {
            nowDis = d;
            chosen = c;
        }
    }

    moveDirX = chosen.x;
    moveDirY = chosen.y;

    if (moveDirX == 1 && moveDirY == 0) faceDir = RIGHT, tickCount_y = 2;
    else if (moveDirX == -1 && moveDirY == 0) faceDir = LEFT, tickCount_y = 1;
    else if (moveDirX == 0 && moveDirY == 1) faceDir = DOWN, tickCount_y = 0;
    else if (moveDirX == 0 && moveDirY == -1) faceDir = UP, tickCount_y = 3;
}







void Ghost::Update(float deltaTime) {
    if (pause_mode) return;

    if (frightenedTimer <= 0.0f && spriteSheet != normalSprite) {
        Speed = 100;
        spriteSheet = normalSprite;
    }

    float centerX = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float centerY = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float distToCenter = std::hypot(Position.x - centerX, Position.y - centerY);

    if (distToCenter < 5.0f) {
        Position.x = centerX;
        Position.y = centerY;

        if (frightenedTimer > 0.0f) {
            escape();
            frightenedTimer -= deltaTime;
        } else {
            setDir();
        }

        int nextX = gridX + moveDirX;
        int nextY = gridY + moveDirY;

        auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
        if (nextX >= 0 && nextX < PlayScene::MapWidth &&
            nextY >= 0 && nextY < PlayScene::MapHeight &&
            scene.map_dot[nextY][nextX] != -1) {
            gridX = nextX;
            gridY = nextY;
        } else {
            moveDirX = moveDirY = 0;
            return;
        }
    }

    Position.x += moveDirX * Speed * deltaTime;
    Position.y += moveDirY * Speed * deltaTime;

    tick++;
    if (tick >= 10) {
        tick = 0;
        tickCount_x = (tickCount_x + 1) % 2;

        if (frightenedTimer > 0.0f) {
            tickCount_x = tickCount_x % 2;
            tickCount_y = 0;
        }
    }

    if (frightenedTimer > 0.0f) {
        frightenedTimer -= deltaTime;
        if (frightenedTimer <= 0.0f) {
            Speed = 100;
            spriteSheet = normalSprite;
            setDir();
        }
    }

    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    float pacX = scene.player->Position.x;
    float pacY = scene.player->Position.y;
    float distToPacman = std::hypot(Position.x - pacX, Position.y - pacY);

    if (distToPacman < 16.0f && coldown > 100) {
        if (scene.player->isPowerMode) {
            Reset();  // 被吃回初始點
            scene.player->money += 200;
            AudioHelper::PlaySample("Pacman/siren.wav");  // 可選
        } else {
            caughtPacman = true;
        }
        coldown = 0;
    }
    coldown++;
    // 防呆：保證鬼的位置始終在合法地圖範圍內

    int gridPosX = static_cast<int>(Position.x) / PlayScene::BlockSize;
    int gridPosY = static_cast<int>(Position.y) / PlayScene::BlockSize;

    if (gridPosX < 0 || gridPosX >= PlayScene::MapWidth ||
        gridPosY < 0 || gridPosY >= PlayScene::MapHeight ||
        scene.map_dot[gridPosY][gridPosX] == -1) {
        // 避免鬼魂走到牆內或出界 → 拉回最近格子中心並停止移動
        gridX = std::max(0, std::min(gridX, PlayScene::MapWidth - 1));
        gridY = std::max(0, std::min(gridY, PlayScene::MapHeight - 1));
        Position.x = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
        Position.y = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
        moveDirX = moveDirY = 0;
    }

}



void Ghost::Draw() const {
    if (Position.x < 0 || Position.y < 0 || Position.x > 1280 || Position.y > 832)
        std::cerr << "[Ghost] Out of screen at (" << Position.x << ", " << Position.y << ")\n";

    if (!spriteSheet) return;

    int bmpW = al_get_bitmap_width(spriteSheet);
    int bmpH = al_get_bitmap_height(spriteSheet);

    int sx = tickCount_x * frameW;
    int sy = (frightenedTimer > 0.0f) ? 0 : tickCount_y * frameH;  // ✅ 核心修正

    if (sx + frameW > bmpW || sy + frameH > bmpH) {
        std::cerr << "[Ghost::Draw] Frame out of bounds: (" << sx << "," << sy << ")\n";
        return;
    }

    float cx = frameW / 2.0f;
    float cy = frameH / 2.0f;
    float drawX = Position.x;
    float drawY = Position.y;

    al_draw_tinted_scaled_rotated_bitmap_region(
        spriteSheet,
        sx, sy, frameW, frameH,
        al_map_rgba(255, 255, 255, 255),
        cx, cy,
        drawX, drawY,
        3.0, 3.0,
        0,
        0
    );
}



void Ghost::escape() {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    std::vector<Engine::Point> candidates;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    Engine::Point prevPos(moveDirX, moveDirY);

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dxs[i];
        int ny = gridY + dys[i];
        if (nx < 0 || nx >= PlayScene::MapWidth || ny < 0 || ny >= PlayScene::MapHeight)
            continue;
        if (scene.map_dot[ny][nx] != -1 &&
            !(dxs[i] == -prevPos.x && dys[i] == -prevPos.y)) {
            candidates.emplace_back(dxs[i], dys[i]);
        }
    }

    if (candidates.empty()) {
        for (int i = 0; i < 4; i++) {
            int nx = gridX + dxs[i];
            int ny = gridY + dys[i];
            if (nx >= 0 && nx < PlayScene::MapWidth &&
                ny >= 0 && ny < PlayScene::MapHeight &&
                scene.map_dot[ny][nx] != -1) {
                candidates.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    if (!isFrightened) {
        candidates.emplace_back(prevPos);
        isFrightened = true;
    }

    int maxDist = -1;
    Engine::Point chosen(0, 0);
    for (auto& dir : candidates) {
        Engine::Point nextPos(gridX + dir.x, gridY + dir.y);
        int d = bfs(nextPos, pacmanPos);
        if (d > maxDist) {
            maxDist = d;
            chosen = dir;
        }
    }

    moveDirX = chosen.x;
    moveDirY = chosen.y;

    if (moveDirX == 1 && moveDirY == 0) faceDir = RIGHT, tickCount_y = 2;
    else if (moveDirX == -1 && moveDirY == 0) faceDir = LEFT, tickCount_y = 1;
    else if (moveDirX == 0 && moveDirY == 1) faceDir = DOWN, tickCount_y = 0;
    else if (moveDirX == 0 && moveDirY == -1) faceDir = UP, tickCount_y = 3;
}

void Ghost::setPacmanPos(const Engine::Point& pos) {
    pacmanPos = Engine::Point(
        static_cast<int>(pos.x) / PlayScene::BlockSize,
        static_cast<int>(pos.y) / PlayScene::BlockSize
    );
}

void Ghost::Reset() {
    Position = Engine::Point(x, y); // 原始建構座標
    gridX = x / PlayScene::BlockSize;
    gridY = y / PlayScene::BlockSize;
    moveDirX = moveDirY = 0;
    frightenedTimer = 0;
    Speed = 100;
    spriteSheet = normalSprite;
    caughtPacman = false;
    coldown = 0;
}*/

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
     gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0), Speed(100) {}

Ghost::~Ghost() {
    if (spriteSheet) {
        al_destroy_bitmap(spriteSheet);
        spriteSheet = nullptr;
    }
}


int Ghost::bfs(Engine::Point A, Engine::Point B) {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    int W = 30;//PlayScene::MapWidth;
    int H = 30;//PlayScene::MapHeight;
    int vis[35][35] = {};
    int dist[35][35] = {};

    int ax = static_cast<int>(A.x);
    int ay = static_cast<int>(A.y);
    int bx = static_cast<int>(B.x);
    int by = static_cast<int>(B.y);

    //if (ax < 0 || ax >= W || ay < 0 || ay >= H || bx < 0 || bx >= W || by < 0 || by >= H)
        //return INT_MAX;

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
            //if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
            nx=(nx+30)%30;
            ny=(ny+30)%30;

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
    vector<Engine::Point> nbrs;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    Engine::Point prevPos(moveDirX, moveDirY);
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dxs[i];
        int ny = gridY + dys[i];
        //if (nx < 0 || nx >= SecondScene::MapWidth || ny < 0 || ny >= SecondScene::MapHeight)
            //continue;
        nx=(nx+30)%30;
        ny=(ny+30)%30;
        
        if (scene.map_dot[ny][nx] != -1&&!frighten) {
            if (!(dxs[i] == -prevPos.x && dys[i] == -prevPos.y)) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    if (nbrs.empty()) {
        for (int i = 0; i < 4; i++) {
            int nx = gridX + dxs[i];
            int ny = gridY + dys[i];
            //if (nx < 0 || nx >= SecondScene::MapWidth || ny < 0 || ny >= SecondScene::MapHeight)
                //continue;
            nx=(nx+30)%30;
            ny=(ny+30)%30;

            if (scene.map_dot[ny][nx] != -1) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
        }
    }
    Engine::Point chosen(0, 0);
    if(!frighten){
    int nowDis = INT_MAX;
    
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d = bfs(nextPos, pacmanPos);  // pacmanPos 必須已經是 grid 座標
        if (d < nowDis) {
            nowDis = d;
            chosen = c;
        }
    }

    if(nowDis>15){
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
    else if(frighten){
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



void Ghost::Update(float deltaTime) {
    if(pause)
        return;

    if(frighten){
        frightenedTimer++;
    }
    if(frighten&&frightenedTimer>300){
        frighten=false;
        Speed=100;
    }

    float centerX = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float centerY = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float distToCenter = std::hypot(Position.x - centerX, Position.y - centerY);

    if (distToCenter < 1.5f) {
        Position.x = centerX;
        Position.y = centerY;
        setDir();
        gridX += moveDirX;
        gridY += moveDirY;

        gridX=(gridX+30)%30;
        gridY=(gridY+30)%30;
    }

    Position.x += moveDirX * Speed * deltaTime;
    Position.y += moveDirY * Speed * deltaTime;

    if (Position.x < 0) Position.x = PlayScene::MapWidth * PlayScene::BlockSize-PlayScene::BlockSize/2;
    if (Position.y < 0) Position.y = PlayScene::MapHeight * PlayScene::BlockSize-PlayScene::BlockSize/2;
    if (Position.x >= PlayScene::MapWidth * PlayScene::BlockSize) 
        Position.x = PlayScene::BlockSize/2;
    if (Position.y >= PlayScene::MapHeight * PlayScene::BlockSize)
        Position.y = PlayScene::BlockSize/2;

    tick++;
    if(tick >= 10) {
        tick = 0;
        tickCount_x = (tickCount_x + 1) % 2;
    }

    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    float pacX = scene.player->Position.x;
    float pacY = scene.player->Position.y;
    float distToPacman = std::hypot(Position.x - pacX, Position.y - pacY);

    if (distToPacman < 16.0f && cold > 100) {
        if(!frighten){
        caught = true;
        cold = 0;
        }
        else if(frighten){
            Reset();
            scene.player->money+=200;
            cold = 0;
        }
    }
    cold++;
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
    Position = Engine::Point(x, y); // 原始建構座標
    gridX = x / PlayScene::BlockSize;
    gridY = y / PlayScene::BlockSize;
    moveDirX = moveDirY = 0;
    frightenedTimer = 0;
    Speed = 100;
    //spriteSheet = normalSprite;
    caught = false;
    cold = 0;
}