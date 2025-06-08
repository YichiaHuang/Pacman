#ifndef INFOSCENE_HPP
#define INFOSCENE_HPP

#include "Engine/IScene.hpp"
#include <allegro5/allegro.h>

class InfoScene : public Engine::IScene {
public:
    InfoScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();

    void Update(float deltaTime) override;
    void Draw() const override;

private:
    ALLEGRO_BITMAP* ghostBitmap = nullptr;
    float ticks = 0;
    int ghostFrame = 0;
    float ghostFrameDuration = 0.3f;

    static constexpr int frameW = 14;
    static constexpr int frameH = 14;
};

#endif
