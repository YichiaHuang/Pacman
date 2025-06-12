#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Clyde.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"

Clyde::Clyde(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/orange_ghost.png");
    normalSprite = spriteSheet;
}

void Clyde::setTargetPos() {
    //std::cout << "[Clyde] setTargetPos called, pacmanPos: (" << pacmanPos.x << "," << pacmanPos.y << ")\n";
    int gx = Position.x / PlayScene::BlockSize;
    int gy = Position.y / PlayScene::BlockSize;
    int px = pacmanPos.x;
    int py = pacmanPos.y;

    int manhattanDis = abs(px - gx) + abs(py - gy);
    if (manhattanDis > 8) {
        targetPos = pacmanPos;
    } else {
        targetPos = Engine::Point(0, 0);
    }
}