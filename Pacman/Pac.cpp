#include "Pac.hpp"
#include "Scene/SecondScene.hpp"
#include "Dot/Dot.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Dot/PowerDot.hpp"
#include <iostream>

Pac::Pac(float x, float y)
    : x(x), y(y), Position(x, y), gridX(x / SecondScene::BlockSize), gridY(y / SecondScene::BlockSize), moveDirX(0), moveDirY(0), Speed(150) {
    spriteSheet = al_load_bitmap("Resource/images/pacman/pac.png");
    if (!spriteSheet) {
        std::cerr << "Failed to load pac.png\n";
    }
    get_hit=false;
}

Pac::~Pac() {
    if (spriteSheet)
        al_destroy_bitmap(spriteSheet);
}

void Pac::Update(float deltaTime) {
    if(scroll_mode)
        scroll_coldown++;
    if(scroll_mode&&scroll_coldown>100){
        Speed_coldown=0;
        scroll_mode=false;
    }
    if(speed_mode)
        Speed_coldown++;
    if(speed_mode&&Speed_coldown>100){
        Speed=150;
        Speed_coldown=0;
        speed_mode=false;
    }
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

                if ((animationFrame == totalFrames - 1 && animationDirection == 1) ||
                    (animationFrame == 0 && animationDirection == -1)) {
                    animationDirection *= -1;
                }

                animationFrame += animationDirection;
            }
        }
    }

    gridX = static_cast<int>(Position.x) / SecondScene::BlockSize;
    gridY = static_cast<int>(Position.y) / SecondScene::BlockSize;

    CheckCollisionWithDots();
}

void Pac::Draw() const {
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
            angle = ALLEGRO_PI / 2;
            break;
        case DOWN:
            angle = -ALLEGRO_PI / 2;
            break;
        default:
            break;
    }
    if(!get_hit){
        al_draw_tinted_scaled_rotated_bitmap_region(
            spriteSheet,
            sx, sy, frameW, frameH,
            al_map_rgba(255, 255, 255, 255),
            cx, cy,
            drawX, drawY,
            1.0, 1.0,
            angle,
            flags
        );
    }
    else{
        al_draw_tinted_scaled_rotated_bitmap_region(
            spriteSheet,
            sx, sy, frameW, frameH,
            al_map_rgba(255, 0, 0, 255),
            cx, cy,
            drawX, drawY,
            1.0, 1.0,
            angle,
            flags
        );
    }
}


void Pac::MoveDirection(int dx, int dy) {
    if (moving) return;

    moveDirX = dx;
    moveDirY = dy;

    int nextGridX = gridX + dx;
    int nextGridY = gridY + dy;

    if (nextGridX < 0 || nextGridX >= SecondScene::MapWidth ||
        nextGridY < 0 || nextGridY >= SecondScene::MapHeight) {
        //return;
        nextGridX=(nextGridX+30)%30;
        nextGridY=(nextGridY+30)%30;
        boundary=true;

    }
    //nextGridX=(nextGridX+30)%30;
    //nextGridY=(nextGridY+30)%30;
    auto& scene = dynamic_cast<SecondScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    if (scene.map_dot[nextGridY][nextGridX] == -1)
        return;

    gridX = nextGridX;
    gridY = nextGridY;
    targetPosition = Engine::Point(gridX * SecondScene::BlockSize + SecondScene::BlockSize / 2,
                                   gridY * SecondScene::BlockSize + SecondScene::BlockSize / 2);
    if (boundary) {
        // 直接瞬移到 target
        Position = targetPosition;
        moving = false;
        if(!scroll_mode)
            boundary=false;
    } else {
        moving = true;
    }

    if (dx == 1) faceDir = RIGHT;
    else if (dx == -1) faceDir = LEFT;
    else if (dy == 1) faceDir = UP;
    else if (dy == -1) faceDir = DOWN;

    moving = true;
}

void Pac::CheckCollisionWithDots() {
    auto& scene = dynamic_cast<SecondScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());
    for (auto& obj : scene.DotsGroup->GetObjects()) {
        Dot* dot = dynamic_cast<Dot*>(obj);
        if (dot && !dot->IsEaten) {
            float dist = std::hypot(Position.x - dot->Position.x, Position.y - dot->Position.y);
            if (dist < 16 && dot->Visible) {
                PowerDot* pd = dynamic_cast<PowerDot*>(dot);
                if (pd) {
                    pd->OnEaten();
                } else {
                    dot->OnEaten();
                }
                AudioHelper::PlaySample("Pacman/pacman_chomp.wav", false, AudioHelper::SFXVolume);
                
                if(dot->effect==0)
                    dotsEaten++;
                
                money+= dot->money;
                
                if(dot->effect==1){
                    Speed=200;
                    Speed_coldown=0;
                    speed_mode=true;
                }
                else if(dot->effect==2){
                    pause=true;
                }
                else if(dot->effect==3)
                {
                    scroll_mode=true;
                    scroll_coldown=0;
                    boundary=true;
                }
                else if(dot->effect==4){
                    power_mode=true;
                    
                }
            }
        }
    }
}
