#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Blin.hpp"
#include "Engine/Point.hpp"

Blin::Blin(float x, float y)
    : GhostSecond(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/red_ghost.png");
    //normalSprite = spriteSheet;
    if (!spriteSheet) {
        std::cerr << "Failed to load blinky.png\n";
    }
}

void Blin::setTargetPos() {
    //targetPos = pacmanPos;
}