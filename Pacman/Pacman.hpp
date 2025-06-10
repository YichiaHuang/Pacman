#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "Engine/Point.hpp"
#include <allegro5/allegro.h>

class Pacman {
public:
    explicit Pacman(float x, float y);
    ~Pacman();
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
    
};

#endif
