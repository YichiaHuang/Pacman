#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Ghost.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"

using namespace std;
using namespace Engine;

Ghost::Ghost(std::string img, float x, float y, float speed)
    : Engine::Sprite(img, x, y, 0.8f, 0.8f, 0.5f, 0.5f){
        pos = Point(x, y);
    };

void Ghost::setPacmanPos(const Engine::Point &pos) {
    pacmanPos = pos;
}

void Ghost::move(std::vector<std::vector<TileType>> mapState) {
    vector<Engine::Point> nbrs;
    static const int dxs[4] = {-1, 1, 0, 0};
    static const int dys[4] = {0, 0, -1, 1};
    for(int i = 0; i < 4; i++) {
        Point nbr = pos;
        nbr.x += dxs[i];
        nbr.y += dys[i];
        if (mapState[int(nbr.y)][int(nbr.x)] != TILE_WALL) {
            nbrs.push_back(nbr);
        }
        int nowDis = INT_MAX;
        Point chosen = pos;
        if (!nbrs.empty()) {
            for(Point& c: nbrs) {
                int d = abs(c.x - targetPos.x) + abs(c.y - targetPos.y);
                if (d < nowDis) {
                    nowDis = d;
                    chosen = c;
                }
            }
        }
    }
}