#include <string>
#include <cmath>

#include "Clyde.hpp"
#include "Engine/Point.hpp"

Clyde::Clyde(float x, float y, float speed)
    : Ghost("clyde.png", x, y, speed) {
}

void Clyde::setTargetPos() {
    int manhattanDis = abs(pacmanPos.x - pos.x) + abs(pacmanPos.y - pos.y);
    if (manhattanDis > 8) {
        targetPos = pacmanPos;
    } else {
        targetPos = Engine::Point(0, 0); //can change to left or right bottom corner 
    }
}