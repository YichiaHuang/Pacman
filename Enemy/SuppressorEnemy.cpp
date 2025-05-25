#include "SuppressorEnemy.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Bullet/DebuffBullet.hpp"
#include "Turret/Turret.hpp"
#include <limits>

SuppressorEnemy::SuppressorEnemy(int x, int y)
    : Enemy("play/enemy-4.png", x, y, 12, 30, 40, 25), fireCooldown(2.5) {
}

void SuppressorEnemy::Update(float deltaTime) {
    fireCooldown -= deltaTime;
    if (fireCooldown <= 0) {
        Turret* nearest = nullptr;
        float minDist = std::numeric_limits<float>::max();
        for (auto& obj : getPlayScene()->TowerGroup->GetObjects()) {
            Turret* t = dynamic_cast<Turret*>(obj);
            float d = (t->Position - Position).Magnitude();
            if (d < 250 && d < minDist) { // 射程範圍 250
                minDist = d;
                nearest = t;
            }
        }
        if (nearest) {
            Engine::Point dir = (nearest->Position - Position).Normalize();
            getPlayScene()->BulletGroup->AddNewObject(
                new DebuffBullet(Position, dir, atan2(dir.y, dir.x), nearest)
            );
        }
        fireCooldown = 2.5; // 每 2.5 秒射一次
    }
    Enemy::Update(deltaTime); // 正常移動
}
