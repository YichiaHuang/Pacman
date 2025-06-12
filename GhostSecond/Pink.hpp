#ifndef PINK_HPP
#define PINK_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "GhostSecond.hpp"

class Pink: public GhostSecond {
    public:
        Pink(float x, float y);
        void setTargetPos(Engine::Point pacmanDir);
};
#endif