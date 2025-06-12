#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Pinky.hpp"
#include "Engine/Point.hpp"

Pinky::Pinky(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/pink_ghost.png");
    normalSprite = spriteSheet;
    if (!spriteSheet) {
        std::cerr << "Failed to load pinky.png\n";
    }
}

void Pinky::setTargetPos(Engine::Point pacmanDir) {
    targetPos.x = pacmanPos.x + pacmanDir.x * 2;
    targetPos.y = pacmanPos.y + pacmanDir.y * 2;
    if (targetPos.x < 0) {
        targetPos.x = 0;
    }
    if (targetPos.y < 0) {
        targetPos.y = 0;
    }

    const int MAP_WIDTH = 20;
    const int MAP_HEIGHT = 13;

    if (targetPos.x >= MAP_WIDTH) { //MAP_WIDTH
        targetPos.x = MAP_WIDTH - 1;
    }
    if (targetPos.y >= MAP_HEIGHT) { //MAP_HEIGHT
        targetPos.y = MAP_HEIGHT - 1;
    }
}