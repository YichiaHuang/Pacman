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
#include "PD.hpp"
#include "Scene/SecondScene.hpp"
#include <iostream>

PD::PD(float x, float y) :
    Dot("play/PowerDot.png", x, y, 1, 4),
    IsEaten(false),
    time(0.0f),
    baseY(y) {
}

void PD::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void PD::OnEaten() {
    if (IsEaten) return;
    IsEaten = true;
    Visible = false;

    /*auto& scene = dynamic_cast<SecondScene&>(*Engine::GameEngine::GetInstance().GetActiveScene());

    for (int i = 0; i < 4; i++) {
        ALLEGRO_BITMAP* frightenedBmp = PlayScene::frightenedBitmap;
        if (!frightenedBmp) {
            std::cerr << "[Error] Failed to load ghost_frighten.png!" << std::endl;
            return;
        }
        //scene.ghost[i]->spriteSheet = frightenedBmp;
        scene.ghost[i]->Speed = 80;
        //scene.ghost[i]->frightenedTimer = 7.0f;

        //scene.ghost[i]->tickCount_y = 0;
    }*/
}