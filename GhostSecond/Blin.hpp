#ifndef BLIN_HPP
#define BLIN_HPP
#include "GhostSecond.hpp"
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "GhostSecond.hpp"

class Blin: public GhostSecond {
    public:
        Blin(float x, float y);
        void setTargetPos();
};
#endif