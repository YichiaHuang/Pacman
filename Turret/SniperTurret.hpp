#ifndef SNIPERTURRET_HPP
#define SNIPERTURRET_HPP
#include "Turret.hpp"

class SniperTurret : public Turret {
public:
    static const int Price;
    SniperTurret(float x, float y);
    void CreateBullet() override;
};

#endif  // SNIPERTURRET_HPP
