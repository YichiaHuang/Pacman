#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scroll.hpp"
#include "Scene/PlayScene.hpp"


Scroll::Scroll(float x, float y) :
    Dot("item/scroll.png", x, y, 0, 3),
    IsEaten(false),
    time(0.0f),
    baseY(y) , money(0), effect(3) {
}

void Scroll::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void Scroll::OnEaten() {
    this->Visible = false;
    
}