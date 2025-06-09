#ifndef PINKY_HPP
#define PINKY_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Ghost.hpp"

class Pinky: public Ghost {
    public:
        Pinky(float x, float y);
        void setTargetPos(Engine::Point pacmanDir);
};
#endif