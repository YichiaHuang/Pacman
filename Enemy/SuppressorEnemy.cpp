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
        //nearest 是最近的砲塔指標
        //minDist 設為無限大，之後用來找最短距離
        Turret* nearest = nullptr;
        float minDist = std::numeric_limits<float>::max();
        for (auto& obj : getPlayScene()->TowerGroup->GetObjects()) {
            Turret* t = dynamic_cast<Turret*>(obj);
            float d = (t->Position - Position).Magnitude();
            if (d < 250 && d < minDist) { // 射程範圍 250
                //比較距離並更新 nearest
                //這樣可以確保只鎖定範圍內最近的砲塔
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

/*
1. 倒數射擊冷卻時間。
2. 冷卻結束後掃描附近砲塔。
3. 找到射程 250 內最近的砲塔。
4. 發射一枚 DebuffBullet 給該砲塔。
5. 重設冷卻時間。
6. 最後呼叫基礎 Enemy::Update() 處理一般的移動行為。
*/
