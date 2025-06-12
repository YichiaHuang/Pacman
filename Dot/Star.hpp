#ifndef STAR_HPP
#define STAR_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Dot/Dot.hpp"

class Star : public Dot {
public:
    bool IsEaten;
    Star(float x, float y);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
    int money;
    int effect;
};
#endif
