#include "Pacman.hpp"
#include "Scene/PlayScene.hpp"
#include "Dot/Dot.hpp"
#include "Engine/GameEngine.hpp"
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

Pacman::Pacman(float x, float y)
    : x(x), y(y), Position(x, y), gridX(x / PlayScene::BlockSize), gridY(y / PlayScene::BlockSize), moveDirX(0), moveDirY(0), Speed(150) {
    spriteSheet = al_load_bitmap("Resource/images/pacman/pac.png");
    if (!spriteSheet) {
        std::cerr << "Failed to load pac.png\n";
    }
}

Pacman::~Pacman() {
    if (spriteSheet)
        al_destroy_bitmap(spriteSheet);
}

void Pacman::Update(float deltaTime) {
    if (moving) {
        Engine::Point direction = targetPosition - Position;
        float distance = direction.Magnitude();

        if (distance < Speed * deltaTime) {
            Position = targetPosition;
            moving = false;
            animationFrame = 0;
        } else {
            direction = direction.Normalize();
            Position.x += direction.x * Speed * deltaTime;
            Position.y += direction.y * Speed * deltaTime;

            animationTimer += deltaTime;
            if (animationTimer >= frameDuration) {
                animationTimer = 0;

                // 先試圖往下一幀，如果預期會超出，就反轉方向
                if ((animationFrame == totalFrames - 1 && animationDirection == 1) ||
                    (animationFrame == 0 && animationDirection == -1)) {
                    animationDirection *= -1;
                }

                animationFrame += animationDirection;
            }


        }
    }

    gridX = static_cast<int>(Position.x) / PlayScene::BlockSize;
    gridY = static_cast<int>(Position.y) / PlayScene::BlockSize;

    CheckCollisionWithDots();
}

void Pacman::Draw() const {
    if (!spriteSheet) return;

    int sx = animationFrame * frameW;
    int sy = 0;
    float cx = frameW / 2.0f;
    float cy = frameH / 2.0f;
    float drawX = Position.x;
    float drawY = Position.y;

    float angle = 0;
    int flags = 0;

    switch (faceDir) {
        case LEFT:
            flags = ALLEGRO_FLIP_HORIZONTAL;
            break;
        case UP:
            angle = ALLEGRO_PI / 2;  // 90 degrees
            break;
        case DOWN:
            angle = -ALLEGRO_PI / 2; // -90 degrees
            break;
        default:
            break;
    }

    al_draw_tinted_scaled_rotated_bitmap_region(
        spriteSheet,
        sx, sy, frameW, frameH,                       // region in source
        al_map_rgba(255, 255, 255, 255),              // no tint
        cx, cy,                                       // rotation center
        drawX, drawY,                                 // destination
        1.0, 1.0,                                     // scaleX, scaleY
        angle,
        flags
    );
}


void Pacman::MoveDirection(int dx, int dy) {
    if (moving) return;

    moveDirX = dx;
    moveDirY = dy;

    int nextGridX = gridX + dx;
    int nextGridY = gridY + dy;

    if (nextGridX < 0 || nextGridX >= PlayScene::MapWidth ||
        nextGridY < 0 || nextGridY >= PlayScene::MapHeight) {
        return;
    }

    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    if (scene.map_dot[nextGridY][nextGridX] == -1)
        return;

    gridX = nextGridX;
    gridY = nextGridY;
    targetPosition = Engine::Point(gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                                   gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2);
    if (dx == 1) faceDir = RIGHT;
    else if (dx == -1) faceDir = LEFT;
    else if (dy == 1) faceDir = UP;
    else if (dy == -1) faceDir = DOWN;

    moving = true;
}

void Pacman::CheckCollisionWithDots() {
    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    for (auto& obj : scene.DotsGroup->GetObjects()) {
        Dot* dot = dynamic_cast<Dot*>(obj);
        if (dot && !dot->IsEaten) {
            float dist = std::hypot(Position.x - dot->Position.x, Position.y - dot->Position.y);
            if (dist < 16 && dot->Visible) {
                dot->OnEaten();
                dotsEaten++;
            }
        }
    }
}
