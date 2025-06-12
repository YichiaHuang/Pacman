#ifndef CLY_HPP
#define CLY_HPP

#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "GhostSecond.hpp"

class Cly: public GhostSecond {
    public:
        Cly(float x, float y);
        void setTargetPos();
};
#endif