#include <string>

#include "Blinky.hpp"
#include "Engine/Point.hpp"

Blinky::Blinky(float x, float y, float speed)
    : Ghost("blinky.png", x, y, speed) {
}

void Blinky::setTargetPos() {
    targetPos = pacmanPos;
}