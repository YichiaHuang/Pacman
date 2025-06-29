#ifndef BLINKY_HPP
#define BLINKY_HPP
#include "Ghost.hpp"
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Ghost.hpp"

class Blinky: public Ghost {
    public:
        Blinky(float x, float y);
        void setTargetPos();
};
#endif