#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "NormalDot.hpp"
#include "Scene/PlayScene.hpp"


NormalDot::NormalDot(float x, float y) :
    Dot("play/dot.png", x, y, 1, 0),
    IsEaten(false),
    time(0.0f),
    baseY(y) {
}

void NormalDot::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void NormalDot::OnEaten() {
    this->Visible = false;
    
}