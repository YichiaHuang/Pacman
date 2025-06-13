#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Inky.hpp"
#include "Engine/Point.hpp"

Inky::Inky(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/blue_ghost.png");
    //normalSprite = spriteSheet;
    predict_mode=true;
    if (!spriteSheet) {
        std::cerr << "Failed to load inky.png\n";
    }
}

void Inky::setTargetPos(Engine::Point pacmanDir,
    Engine::Point blinkyPos) {

    /*Engine::Point pacmanFront(
        pacmanPos.x + pacmanDir.x * 2,
        pacmanPos.y + pacmanDir.y * 2
    );

    Engine::Point vec(
        pacmanFront.x - blinkyPos.x,
        pacmanFront.y - blinkyPos.y
    );

    Engine::Point rawTarget(
        blinkyPos.x + vec.x * 2,
        blinkyPos.y + vec.y * 2
    );

    const int MAP_WIDTH = 20;
    const int MAP_HEIGHT = 13;
    targetPos.x = std::max(1.0f, std::min(rawTarget.x, float(MAP_WIDTH - 1)));
    targetPos.y = std::max(1.0f, std::min(rawTarget.y, float(MAP_HEIGHT - 1)));*/
}