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
    Dot(std::string img, float x, float y, int money, int effect);
    void OnEaten();
    void Update(float deltaTime) override;
    float time;
    int baseY;
    int money;
    int effect;
};
#endif
