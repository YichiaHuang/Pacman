#ifndef GHOST_HPP
#define GHOST_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Ghost: public Engine::Sprite {
    public:
        Ghost(std::string img, float x, float y, float speed);
        void setPacmanPos(const Engine::Point& pos);
        virtual void setTargetPos();
        enum TileType {
            TILE_WALL,
            TILE_FLOOR,
        };
        void move(std::vector<std::vector<TileType>> mapState);
    protected:
        Engine::Point pacmanPos;
        Engine::Point targetPos;
        Engine::Point pos;
};
#endif // GHOST_HPP