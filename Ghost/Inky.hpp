#ifndef INKY_HPP
#define INKY_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Ghost.hpp"

class Inky: public Ghost {
    public:
        Inky(float x, float y);
        void setTargetPos(Engine::Point PacmanDir,
             Engine::Point BlinkyPos);
};
#endif