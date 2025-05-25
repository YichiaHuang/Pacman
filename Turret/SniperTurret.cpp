#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "SniperTurret.hpp"
#include "Bullet/FireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"

const int SniperTurret::Price = 150;

SniperTurret::SniperTurret(float x, float y)
    : Turret("play/tower-base.png", "play/turret-3.png", x, y, 500, Price, 0.8) {
    Anchor.y += 8.0f / GetBitmapHeight();
}

void SniperTurret::CreateBullet() {
    if (!Target) return;

    Engine::Point dir = Target->Position - Position;
    float baseAngle = atan2(dir.y, dir.x);  // 中心角度
    float offsets[3] = { 0, ALLEGRO_PI / 12, -ALLEGRO_PI / 12 }; // 0°, +15°, -15°

    for (float offset : offsets) {
        float angle = baseAngle + offset;
        Engine::Point bulletDir(cos(angle), sin(angle));
        getPlayScene()->BulletGroup->AddNewObject(
            new FireBullet(Position + bulletDir * 36, bulletDir, angle, this));
    }

    AudioHelper::PlayAudio("gun.wav");
}