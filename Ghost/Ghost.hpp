#ifndef GHOST_HPP
#define GHOST_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/IObject.hpp"

class Ghost : public Engine::IObject{
public:
    Ghost(float x, float y);
    virtual ~Ghost();
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    void setDir();
    void setPacmanPos(const Engine::Point &pos);
    //virtual const char* GetSpriteName() const = 0;
    Engine::Point GetPosition() const { return Position; }
    int animationDirection = 1;

protected:
    float Speed;
    float x, y;
    Engine::Point Position;
    Engine::Point targetPos;
    Engine::Point pacmanPos;
    int moveDirX, moveDirY;
    int gridX, gridY;

    ALLEGRO_BITMAP* spriteSheet = nullptr;
    int animationFrame = 0;
    float animationTimer = 0;
    const int totalFrames = 2;
    const float frameDuration = 0.1f;
    const int frameW = 14;
    const int frameH = 14;

    enum Direction { RIGHT = 0, LEFT, UP, DOWN };
    Direction faceDir = RIGHT;

    int tick=0;
    int tickCount_x = 0;
    int tickCount_y = 0;
};
#endif // GHOST_HPP