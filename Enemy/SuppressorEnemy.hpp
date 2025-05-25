#ifndef SUPPRESSORENEMY_HPP
#define SUPPRESSORENEMY_HPP
#include "Enemy.hpp"

class SuppressorEnemy : public Enemy {
private:
    float fireCooldown; // 發射子彈的冷卻時間

public:
    SuppressorEnemy(int x, int y);
    void Update(float deltaTime) override;
};

#endif
