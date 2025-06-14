#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Inky.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/GameEngine.hpp"
Inky::Inky(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/blue_ghost.png");
    //normalSprite = spriteSheet;
    predict_mode=true;
    if (!spriteSheet) {
        std::cerr << "Failed to load inky.png\n";
    }
}

void Inky::setTargetPos(/*Engine::Point pacmanDir,
    Engine::Point blinkyPos*/) {
    int gx = Position.x / PlayScene::BlockSize;
    int gy = Position.y / PlayScene::BlockSize;
    int px = pacmanPos.x;
    int py = pacmanPos.y;

    int manhattanDis = abs(px - gx) + abs(py - gy);
    if (manhattanDis > 4) {
        
        predict_mode=true;
    } else {
        //targetPos = Engine::Point(0, 0);
        //pacmanPos=Engine::Point(1, 1);
        predict_mode=false;
        
    }
    
    
    /*Engine::Point pacmanFront(
        pacmanPos.x + pacmanDir.x * 2,
        pacmanPos.y + pacmanDir.y * 2
    );

    Engine::Point vec(
        pacmanFront.x - blinkyPos.x,
        pacmanFront.y - blinkyPos.y
    );

    Engine::Point rawTarget(
        blinkyPos.x + vec.x * 2,
        blinkyPos.y + vec.y * 2
    );

    const int MAP_WIDTH = 20;
    const int MAP_HEIGHT = 13;
    targetPos.x = std::max(1.0f, std::min(rawTarget.x, float(MAP_WIDTH - 1)));
    targetPos.y = std::max(1.0f, std::min(rawTarget.y, float(MAP_HEIGHT - 1)));*/
}