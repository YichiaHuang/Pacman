#ifndef NORMALDOT_HPP
#define NORMALDOT_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Dot/Dot.hpp"

class NormalDot : public Dot {
public:
    bool IsEaten;
    NormalDot(float x, float y);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
    int money;
    int effect;
};
#endif
