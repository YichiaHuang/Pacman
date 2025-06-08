#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Pacman.hpp"


#include "Dot/Dot.hpp"
#include "Scene/PlayScene.hpp"
#include <cmath>
#include <allegro5/allegro_primitives.h>

Pacman::Pacman(float x, float y) :
    Engine::Sprite("pacman/pac_test.png", x, y, 32, 32, 0.5, 0.5),
    Speed(150), gridX(x / PlayScene::BlockSize), gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0) {
}

void Pacman::Update(float deltaTime) {
    
    
    
    
    
    
    if (moving) {
        Engine::Point direction = targetPosition - Position;
        float distance = direction.Magnitude();

        if (distance < Speed * deltaTime) {
            Position = targetPosition;
            moving = false;
        } else {
            direction = direction.Normalize();
            Position.x += direction.x * Speed * deltaTime;
            Position.y += direction.y * Speed * deltaTime;
        }
    }

    gridX = static_cast<int>(Position.x) / PlayScene::BlockSize;
    gridY = static_cast<int>(Position.y) / PlayScene::BlockSize;
    CheckCollisionWithDots();
    Sprite::Update(deltaTime);
}

void Pacman::MoveDirection(int dx, int dy) {
    if (moving) return;

    moveDirX = dx;
    moveDirY = dy;

    int nextGridX = gridX + dx;
    int nextGridY = gridY + dy;

    // Check bounds
    if (nextGridX < 0 || nextGridX >= PlayScene::MapWidth ||
        nextGridY < 0 || nextGridY >= PlayScene::MapHeight) {
        return;
    }

    // Optional: Check for wall
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    if (scene.map_dot[nextGridY][nextGridX] == -1) { // assume -1 = wall
        return;
    }

    gridX = nextGridX;
    gridY = nextGridY;
    targetPosition = Engine::Point(gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                   gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
    moving = true;
}

void Pacman::CheckCollisionWithDots() {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    for (auto& obj : scene.DotsGroup->GetObjects()) {
        Dot* dot = dynamic_cast<Dot*>(obj);
        if (dot && !dot->IsEaten) {
            float dist = std::hypot(Position.x - dot->Position.x, Position.y - dot->Position.y);
            if (dist < 16) {
                if(dot->Visible){
                    dot->OnEaten();
                    dotsEaten++;
                }
                
            }
        }
    }
}