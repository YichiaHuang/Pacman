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
#include "PowerDot.hpp"
#include "Scene/PlayScene.hpp"
#include <iostream>

PowerDot::PowerDot(float x, float y) :
    Dot("play/PowerDot.png", x, y, 1, 0),
    IsEaten(false),
    time(0.0f),
    baseY(y) {
}

void PowerDot::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void PowerDot::OnEaten() {
    if (IsEaten) return;
    IsEaten = true;
    Visible = false;

    auto& scene = dynamic_cast<PlayScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    for (int i = 0; i < 4; i++) {
        ALLEGRO_BITMAP* newFrightened = al_load_bitmap("Resource/images/ghost/ghost_frighten.png");
        if (!newFrightened) {
            std::cerr << "[Error] Failed to load ghost_frighten.png!" << std::endl;
            continue;
        }
        if (scene.ghost[i]->spriteSheet && scene.ghost[i]->spriteSheet != scene.ghost[i]->normalSprite) {
            al_destroy_bitmap(scene.ghost[i]->spriteSheet);
        }
        scene.ghost[i]->spriteSheet = newFrightened;
        scene.ghost[i]->Speed = 80;
        scene.ghost[i]->frightenedTimer = 7.0f;


    }
}