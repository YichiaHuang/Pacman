#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Speed.hpp"
#include "Scene/PlayScene.hpp"


Speed::Speed(float x, float y) :
    Dot("item/speed.png", x, y, 0, 1),
    IsEaten(false),
    time(0.0f),
    baseY(y){
}

void Speed::Update(float deltaTime) {
    time += deltaTime;
    Position.y = baseY + std::sin(time * 2) * 1.5; // 每秒上下飄動約 10 pixels（幅度 5）
}

void Speed::OnEaten() {
    this->Visible = false;
    
}