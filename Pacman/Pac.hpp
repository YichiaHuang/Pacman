#ifndef PAC_HPP
#define PAC_HPP

#include "Engine/Point.hpp"
#include <allegro5/allegro.h>

class Pac {
public:
    explicit Pac(float x, float y);
    ~Pac();
    void Update(float deltaTime);
    void Draw() const;
    void MoveDirection(int dx, int dy);
    void CheckCollisionWithDots();
    Engine::Point GetPosition() const { return Position; }
    int animationDirection = 1;
    int dotsEaten = 0;
    int money=0;

    Engine::Point Position;
    bool get_hit=false;
    bool pause = false;
private:
    float Speed;
    float x, y;
    
    Engine::Point targetPosition;
    int gridX, gridY;
    int moveDirX, moveDirY;
    bool moving = false;

    ALLEGRO_BITMAP* spriteSheet = nullptr;
    int animationFrame = 0;
    float animationTimer = 0;
    const int totalFrames = 3;
    const float frameDuration = 0.1f;
    const int frameW = 33;
    const int frameH = 34;

    enum Direction { RIGHT = 0, LEFT, UP, DOWN };
    Direction faceDir = RIGHT;
    int Speed_coldown=0;
    bool speed_mode = false;
    bool boundary=false;
    bool scroll_mode=false;
    int scroll_coldown=0;
};

#endif
