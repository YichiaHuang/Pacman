#ifndef SPEED_HPP
#define SPEED_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Dot/Dot.hpp"

class Speed : public Dot {
public:
    bool IsEaten;
    Speed(float x, float y);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
    int money;
    int effect;
};
#endif
