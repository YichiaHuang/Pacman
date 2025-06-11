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

/*void Ghost::setPacmanPos(const Engine::Point &pos) {
    pacmanPos = pos;
}

void Ghost::setDir() {
    vector<Engine::Point> nbrs;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    Engine::Point prevPos;
    prevPos.x = moveDirX;
    prevPos.y = moveDirY;
    for(int i = 0; i < 4; i++) {
        Point nbr;
        nbr.x = dxs[i];
        nbr.y = dys[i];
        auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
        if (scene.map_dot[int(nbr.y)][int(nbr.x)] != -1) {
            if (!(nbr.x == prevPos.x && nbr.y == prevPos.y)) {
                nbrs.push_back(nbr);
            }
        }
    }

    if (nbrs.empty()) {
        for(int i = 0; i < 4; i++) {
            Point nbr;
            nbr.x = dxs[i];
            nbr.y = dys[i];
            auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
            if (scene.map_dot[int(nbr.y)][int(nbr.x)] != -1) {
                nbrs.push_back(nbr);
            }
        }
    }

    int nowDis = INT_MAX;
    Point chosen = Point(0, 0);
    if (!nbrs.empty()) {
        for(Point& c: nbrs) {
            Engine::Point nextPos = Engine::Point(Position.x + c.x, Position.y + c.y);
            int d = bfs(nextPos, targetPos);
            if (d < nowDis) {
                nowDis = d;
                chosen = c;
            }
        }
    }
    moveDirX = chosen.x;
    moveDirY = chosen.y;

    if (moveDirX == 1 && moveDirY == 0) {
        faceDir = RIGHT;
        tickCount_y = 2;
    } else if (moveDirX == -1 && moveDirY == 0) {
        faceDir = LEFT;
        tickCount_y = 1;
    } else if (moveDirX == 0 && moveDirY == 1) {
        faceDir = UP;
        tickCount_y = 3;
    } else if (moveDirX == 0 && moveDirY == -1) {
        faceDir = DOWN;
        tickCount_y = 0;
    }
}*/




/*int bfs(Engine::Point A, Engine::Point B) {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    int vis[50][50], dist[50][50];
    memset(vis, 0, sizeof(vis));
    memset(dist, 0, sizeof(dist));
    int dx[4]={1, 0, -1, 0};
        int dy[4]={0, 1, 0, -1};
    dist[(int)A.x][(int)A.y] = 0;
    queue<Engine::Point> q;
    q.push(A);
    while (!q.empty()) {
        Engine::Point cur = q.front();
        q.pop();
        if (vis[(int)cur.x][(int)cur.y]) continue;
        if (cur.x == B.x && cur.y == B.y) {
            return dist[(int)cur.x][(int)cur.y];
        }
        vis[(int)cur.x][(int)cur.y] = 1;
        for (int i = 0; i < 4; i++) {
            Engine::Point next = cur;
            next.x += dx[i];
            next.y += dy[i];
            if (next.x < 0 || next.x >= PlayScene::MapWidth || next.y < 0 || next.y >= PlayScene::MapHeight) continue;
            if (scene.map_dot[(int)next.y][(int)next.x] == -1) continue; //not wall
            if (!vis[(int)next.x][(int)next.y]) {
                dist[(int)next.x][(int)next.y] = dist[(int)cur.x][(int)cur.y] + 1;
                q.push(next);
            }
        }
    }
    return INT_MAX; // unreachable
}



void Ghost::Update(float deltaTime) {
    int blockCenterX = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    int blockCenterY = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float distToCenter = std::hypot(Position.x - blockCenterX, Position.y - blockCenterY);

    if (distToCenter < 2.0f) {
        Position.x = blockCenterX;
        Position.y = blockCenterY;
        if(frightenedTimer == 0) {
            setDir();
        }
        else {
            // If frightened, move away from Pacman
            escape();
            frightenedTimer -= deltaTime;
        }
        gridX += moveDirX;
        gridY += moveDirY;
    }

    Position.x += moveDirX * Speed * deltaTime;
    Position.y += moveDirY * Speed * deltaTime;

    tick++;
    if(tick>=10)
    {
        tick=0;
        tickCount_x=(tickCount_x+1)%2;
    }
}*/








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
        if (scene.map_dot[ny][nx] != -1) {
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
            if (scene.map_dot[ny][nx] != -1) {
                nbrs.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    int nowDis = INT_MAX;
    Engine::Point chosen(0, 0);
    for (auto& c : nbrs) {
        Engine::Point nextPos(gridX + c.x, gridY + c.y);
        int d = bfs(nextPos, targetPos);  // pacmanPos 必須已經是 grid 座標
        //std::cout << "[Clyde] bfs from (" << nextPos.x << "," << nextPos.y << ") to (" << targetPos.x << "," << targetPos.y << ") = " << d << "\n";
        if (d < nowDis) {
            nowDis = d;
            chosen = c;
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
    if(pause_mode){
        return;
    }
    float centerX = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float centerY = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    float distToCenter = std::hypot(Position.x - centerX, Position.y - centerY);

    if (distToCenter < 1.5f) {
        Position.x = centerX;
        Position.y = centerY;

        if (frightenedTimer > 0.0f) {
            escape();
            frightenedTimer -= deltaTime;
        } else {
            setDir();
            // 如果之前是逃跑狀態，改回正常速度與圖示
            if (frightenedTimer <= 0.0f && spriteSheet != normalSprite) {
                Speed = 100;
                if (spriteSheet) {
                    al_destroy_bitmap(spriteSheet);  // only destroy frighten.png
                }
                spriteSheet = normalSprite;
            }

        }

        gridX += moveDirX;
        gridY += moveDirY;
    }


    Position.x += moveDirX * Speed * deltaTime;
    Position.y += moveDirY * Speed * deltaTime;

    tick++;
    if(tick >= 10) {
        tick = 0;
        tickCount_x = (tickCount_x + 1) % 2;
    }

    // 檢查是否與 Pacman 接觸
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    float pacX = scene.player->Position.x;
    float pacY = scene.player->Position.y;
    float distToPacman = std::hypot(Position.x - pacX, Position.y - pacY);

    if (distToPacman < 16.0f&&coldown>100) { // 調整閾值以適應角色大小
        caughtPacman = true;
        coldown=0;
    }
    
        coldown++;
        
    
}
void Ghost::setPacmanPos(const Engine::Point &pos) {
    // 傳入前請確保 pacmanPos 是格子座標
    pacmanPos = Engine::Point(
        static_cast<int>(pos.x) / PlayScene::BlockSize,
        static_cast<int>(pos.y) / PlayScene::BlockSize
    );
}



void Ghost::Draw() const {
    
    
    float cx = frameW / 2.0f;
    float cy = frameH / 2.0f;
    float drawX = Position.x;
    float drawY = Position.y;

    al_draw_tinted_scaled_rotated_bitmap_region(
        spriteSheet,
        tickCount_x * frameW, tickCount_y * frameH, frameW, frameH,                       // region in source
        al_map_rgba(255, 255, 255, 255),              // no tint
        cx, cy,                                       // rotation center
        drawX, drawY,                                 // destination
        3.0, 3.0,                                     // scaleX, scaleY
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
        if (scene.map_dot[ny][nx] != -1) {
            if (!(dxs[i] == -prevPos.x && dys[i] == -prevPos.y)) {
                candidates.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    if (candidates.empty()) {
        for (int i = 0; i < 4; i++) {
            int nx = gridX + dxs[i];
            int ny = gridY + dys[i];
            if (nx < 0 || nx >= PlayScene::MapWidth || ny < 0 || ny >= PlayScene::MapHeight)
                continue;
            if (scene.map_dot[ny][nx] != -1) {
                candidates.emplace_back(dxs[i], dys[i]);
            }
        }
    }

    int maxDist = -1;
    Engine::Point chosen(0, 0);
    for (auto& dir : candidates) {
        Engine::Point nextPos(gridX + dir.x, gridY + dir.y);
        int d = bfs(nextPos, pacmanPos); // pacmanPos 早已設定為格子座標
        if (d > maxDist) {
            maxDist = d;
            chosen = dir;
        }
    }

    moveDirX = chosen.x;
    moveDirY = chosen.y;

    // 更新動畫方向（可選擇是否照做）
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
