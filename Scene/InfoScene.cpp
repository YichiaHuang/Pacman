#include "InfoScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

void InfoScene::Initialize() {
    ticks = 0;
    lastFrameSwitch = 0;
    ghostFrame = 0;
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    AddNewObject(new Engine::Label("Ghost Info", "prstartk.ttf", 64, w / 2, 100, 255, 255, 255, 255, 0.5, 0.5));

    ghostBitmap = al_load_bitmap("Resource/images/ghost/ghosts.png");
    if (!ghostBitmap) {
        std::cerr << "Failed to load images/ghost/ghosts.png\n";
    }

    // 鬼資料
    struct Ghost {
        const char* name;
        ALLEGRO_COLOR color;
    };

    Ghost ghosts[] = {
        {"BLINKY (Red)",    al_map_rgb(255, 0, 0)},
        {"PINKY (Pink)",    al_map_rgb(255, 105, 180)},
        {"INKY (Cyan)",     al_map_rgb(0, 255, 255)},
        {"CLYDE (Orange)",  al_map_rgb(255, 165, 0)},
    };

    int baseY = 250;
    int lineSpacing = 80;

    for (int i = 0; i < 4; ++i) {
        int labelY = baseY + i * lineSpacing;
        AddNewObject(new Engine::Label(
            ghosts[i].name, "prstartk.ttf", 32,
            w / 2, labelY,
            ghosts[i].color.r * 255, ghosts[i].color.g * 255, ghosts[i].color.b * 255,
            255, 0.5, 0));
    }

    auto btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w / 2 - 100, h - 120, 200, 70);
    btn->SetOnClickCallback(std::bind(&InfoScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "prstartk.ttf", 36, w / 2, h - 85, 0, 0, 0, 255, 0.5, 0.5));
}

void InfoScene::Terminate() {
    if (ghostBitmap) {
        al_destroy_bitmap(ghostBitmap);
        ghostBitmap = nullptr;
    }
    IScene::Terminate();
}

void InfoScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void InfoScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks - lastFrameSwitch >= ghostFrameDuration) {
        lastFrameSwitch += ghostFrameDuration;
        ghostFrame = (ghostFrame + 1) % 2;
    }
}

void InfoScene::Draw() const {
    IScene::Draw();

    if (!ghostBitmap) return;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int baseY = 250;
    int spacingY = 80;
    int scale = 3;

    // 每隻鬼「向左走」動畫所在行
    const int ghostYRows[] = {2, 14, 10, 6};  // Red, Pink, Cyan, Orange

    int numToShow = std::min(4, static_cast<int>(ticks / 1.5f));

    for (int i = 0; i < numToShow; ++i) {
        int frameX = ghostFrame * frameW;
        int frameY = ghostYRows[i] * frameH;

        int drawX = w / 2 - 400;
        int drawY = baseY + i * spacingY - (frameH * scale) / 2 + 20;

        al_draw_scaled_bitmap(
            ghostBitmap,
            frameX, frameY,
            frameW, frameH,
            drawX, drawY,
            frameW * scale, frameH * scale,
            0
        );
    }
}


