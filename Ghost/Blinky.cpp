#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Blinky.hpp"
#include "Engine/Point.hpp"

Blinky::Blinky(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/red_ghost.png");
    //normalSprite = spriteSheet;
    if (!spriteSheet) {
        std::cerr << "Failed to load blinky.png\n";
    }
}

void Blinky::setTargetPos() {
    //targetPos = pacmanPos;
}