#ifndef PD_HPP
#define PD_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Dot/Dot.hpp"

class PD : public Dot {
public:
    bool IsEaten;
    PD(float x, float y);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
    int money;
    int effect;
};
#endif
