#ifndef CLYDE_HPP
#define CLYDE_HPP

#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Ghost.hpp"

class Clyde: public Ghost {
    public:
        Clyde(float x, float y);
        void setTargetPos();
};
#endif