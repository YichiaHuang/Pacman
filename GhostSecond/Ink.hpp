#ifndef INK_HPP
#define INK_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "GhostSecond.hpp"

class Ink: public GhostSecond {
    public:
        Ink(float x, float y);
        void setTargetPos(Engine::Point PacmanDir,
             Engine::Point BlinkyPos);
};
#endif