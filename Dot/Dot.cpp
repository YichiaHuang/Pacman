#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Dot.hpp"
#include "Scene/PlayScene.hpp"


Dot::Dot(float x, float y) :
    Engine::Sprite("play/dot.png", x, y, 32, 32, 0.5, 0.5),
    IsEaten(false),
    time(0.0f),
    baseY(y) {
}

void Dot::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void Dot::OnEaten() {
    this->Visible = false;
    
}