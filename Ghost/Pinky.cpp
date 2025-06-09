#include <string>

#include "Pinky.hpp"
#include "Engine/Point.hpp"

Pinky::Pinky(float x, float y, float speed)
    : Ghost("pinky.png", x, y, speed) {
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

    const int MAP_WIDTH = 32;
    const int MAP_HEIGHT = 28;

    if (targetPos.x >= MAP_WIDTH) { //MAP_WIDTH
        targetPos.x = MAP_WIDTH - 1;
    }
    if (targetPos.y >= MAP_HEIGHT) { //MAP_HEIGHT
        targetPos.y = MAP_HEIGHT - 1;
    }
}