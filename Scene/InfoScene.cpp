#include "InfoScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

void InfoScene::Initialize() {
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
        int xIndex; // 每個鬼的起始列（以2格為單位）
    };

    Ghost ghosts[] = {
        {"BLINKY (Red)",    al_map_rgb(255, 0, 0),     0},  // x = 0
        {"PINKY (Pink)",    al_map_rgb(255, 105, 180), 2},  // x = 28
        {"INKY (Cyan)",     al_map_rgb(0, 255, 255),   4},  // x = 56
        {"CLYDE (Orange)",  al_map_rgb(255, 165, 0),   6},  // x = 84
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
    if (ticks >= ghostFrameDuration) {
        ticks = 0;
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

    for (int i = 0; i < 4; ++i) {
        int ghostWidth = 2 * frameW; // 每隻鬼佔 2 格
        int frameX = ghostFrame * frameW + i * ghostWidth;
        int frameY = 2 * frameH;  // 第三行（向左）

        int drawX = w / 2 - 400;  // 放在 label 左邊
        int drawY = baseY + i * spacingY - (frameH * scale) / 2+20;

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

