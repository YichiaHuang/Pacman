#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "DebuffBullet.hpp"
#include "Turret/Turret.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Collider.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include <allegro5/color.h>
#include <allegro5/allegro.h>
#include <thread>
#include <chrono>


DebuffBullet::DebuffBullet(Engine::Point pos, Engine::Point dir, float rot, Turret* target)
    : Bullet("play/bullet-8.png", 400, 1, pos, dir, rot, nullptr), TargetTurret(target) {
}

void DebuffBullet::ApplyDebuff() {
    if (TargetTurret) {
        TargetTurret->slowTimer = 3.0f;
        TargetTurret->cooldownModifier = 2.0f;

        // 視覺效果：紅色閃爍三次（異步處理以避免主線程阻塞）
        std::thread([turret = TargetTurret]() {
            for (int i = 0; i < 3; ++i) {
                turret->Tint = al_map_rgba(255, 0, 0, 255);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                turret->Tint = al_map_rgba(255, 255, 255, 255);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }).detach();
    }
}


void DebuffBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    if (!TargetTurret || !TargetTurret->Enabled) return;

    float dist = (TargetTurret->Position - Position).Magnitude();
    if (dist <= CollisionRadius + 4) {
        ApplyDebuff();
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }

    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2,
        Engine::Point(0, 0), PlayScene::GetClientSize())) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
    }
}

