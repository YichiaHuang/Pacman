#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Star.hpp"
#include "Scene/PlayScene.hpp"


Star::Star(float x, float y) :
    Dot("item/star.png", x, y, 3, -1),
    IsEaten(false),
    time(0.0f),
    baseY(y) , money(3), effect(0) {
}

void Star::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void Star::OnEaten() {
    this->Visible = false;
    
}