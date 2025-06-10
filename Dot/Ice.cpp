#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Ice.hpp"
#include "Scene/PlayScene.hpp"


Ice::Ice(float x, float y) :
    Dot("item/ice.png", x, y, 0, 2),
    IsEaten(false),
    time(0.0f),
    baseY(y) {
}

void Ice::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void Ice::OnEaten() {
    this->Visible = false;
    
}