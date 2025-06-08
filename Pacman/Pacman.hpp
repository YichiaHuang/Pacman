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

private:
    float Speed;
    int gridX, gridY;
    int moveDirX, moveDirY;
    bool moving = false;
    Engine::Point targetPosition;
};
#endif
