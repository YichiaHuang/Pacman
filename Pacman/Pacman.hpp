#ifndef PACMAN_HPP
#define PACMAN_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

// Pacman.hpp
class Pacman : public Engine::Sprite {
public:
    explicit Pacman(float x, float y);
    void Update(float deltaTime) override;
    void MoveDirection(int dx, int dy);
    void CheckCollisionWithDots();
    int animationFrame = 0;
    float animationTimer = 0;
    const int totalFrames = 3;
    const float frameDuration = 0.1f; // 每幀顯示 0.1 秒
    int dotsEaten = 0;


private:
    float Speed;
    int gridX, gridY;
    int moveDirX, moveDirY;
    bool moving = false;
    Engine::Point targetPosition;
};
#endif
