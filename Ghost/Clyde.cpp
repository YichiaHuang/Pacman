#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "Clyde.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"

Clyde::Clyde(float x, float y)
    : Ghost(x, y) {
    spriteSheet = al_load_bitmap("Resource/images/ghost/orange_ghost.png");
    //normalSprite = spriteSheet;
}

void Clyde::setTargetPos() {
    //std::cout << "[Clyde] setTargetPos called, pacmanPos: (" << pacmanPos.x << "," << pacmanPos.y << ")\n";
    int gx = Position.x / PlayScene::BlockSize;
    int gy = Position.y / PlayScene::BlockSize;
    int px = pacmanPos.x;
    int py = pacmanPos.y;
    if(flee){
        flee_timer++;
    }
    if(flee&&flee_timer>300){
        flee=false;
        first_step=true;
    }
        

    int manhattanDis = abs(px - gx) + abs(py - gy);
    if (manhattanDis > 4) {
        //targetPos = pacmanPos;
        //flee=false;
        flag=1;
    } else {
        //targetPos = Engine::Point(0, 0);
        //pacmanPos=Engine::Point(1, 1);
        if(flag){
        flee=true;
        first_step=true;
        flag=0;
        flee_timer=0;
        }
    }
}