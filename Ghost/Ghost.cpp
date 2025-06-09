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
     gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0), Speed(150) {}

Ghost::~Ghost() {
    if (spriteSheet) {
        al_destroy_bitmap(spriteSheet);
        spriteSheet = nullptr;
    }
}

void Ghost::setPacmanPos(const Engine::Point &pos) {
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
}

int bfs(Engine::Point A, Engine::Point B) {
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
        setDir();
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
}

void Ghost::Draw() const {

    int sx = tickCount_x * frameW;
    int sy = tickCount_y * frameH;
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