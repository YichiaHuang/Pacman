#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Clyde.hpp"
#include "Engine/Point.hpp"

Clyde::Clyde(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/orange_ghost.png");
}

void Clyde::setTargetPos() {
    int manhattanDis = abs(pacmanPos.x - Position.x) + abs(pacmanPos.y - Position.y);
    if (manhattanDis > 8) {
        targetPos = pacmanPos;
    } else {
        targetPos = Engine::Point(0, 0); //can change to left or right bottom corner 
    }
}