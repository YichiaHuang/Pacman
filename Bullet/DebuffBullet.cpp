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
        TargetTurret->slowTimer = 3.0f;// 3秒內被減速
        TargetTurret->cooldownModifier = 2.0f;// 發射冷卻時間變成 2 倍

        // 視覺效果：紅色閃爍三次（異步處理以避免主線程阻塞）
        /*
        使用 lambda 函式 + thread 非同步控制砲塔的閃爍。
        al_map_rgba(...) 設定 Tint 為紅色或白色，達到閃爍視覺效果。
        sleep_for(100ms)：每次紅白交錯閃爍 100 毫秒，共閃三次。
        detach()：背景執行，不會阻塞主執行緒。
        */
        std::thread([turret = TargetTurret]() {
            for (int i = 0; i < 3; ++i) {
                turret->Tint = al_map_rgba(255, 0, 0, 255);//red
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                turret->Tint = al_map_rgba(255, 255, 255, 255);//original color
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

