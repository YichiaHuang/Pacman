#ifndef DEBUFFBULLET_HPP
#define DEBUFFBULLET_HPP
#include "Bullet.hpp"

class Turret;

class DebuffBullet : public Bullet {
private:
    Turret* TargetTurret;

public:
    DebuffBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* target);
    void ApplyDebuff();
    void Update(float deltaTime) override;
};

#endif