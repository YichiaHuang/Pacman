#ifndef DOT_HPP
#define DOT_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Dot : public Engine::Sprite {
public:
    bool IsEaten;
    Dot(float x, float y);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
};
#endif
