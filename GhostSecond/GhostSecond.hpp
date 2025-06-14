#ifndef GHOSTSECOND_HPP
#define GHOSTSECOND_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/IObject.hpp"

class GhostSecond : public Engine::IObject{
public:
    GhostSecond(float x, float y);
    virtual ~GhostSecond();
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    void setDir();
    void setPacmanPos(const Engine::Point &pos);
    //virtual const char* GetSpriteName() const = 0;
    Engine::Point GetPosition() const { return Position; }
    int animationDirection = 1;
    int bfs(Engine::Point A, Engine::Point B);
    //static Engine::Point CameraPos;
    bool pause=false;
    bool caught=false;
    void Reset();
    int frightenedTimer;
    bool frighten=false;
    float Speed;
    bool f_firststep=false;
    bool first_step=false;
    bool return_mode=false;
    bool return_pre=false;
    int originX;
    int originY;
    Engine::Point origin;
protected:
    
    float x, y;
    Engine::Point Position;
    Engine::Point targetPos;
    Engine::Point pacmanPos;
    int moveDirX, moveDirY;
    int gridX, gridY;
    
    ALLEGRO_BITMAP* spriteSheet = nullptr;
    ALLEGRO_BITMAP* ScareSheet = al_load_bitmap("Resource/images/ghost/ghost_scared.png");
    ALLEGRO_BITMAP* FrightenSheet = al_load_bitmap("Resource/images/ghost/ghost_frighten.png");
    int animationFrame = 0;
    float animationTimer = 0;
    const int totalFrames = 2;
    const float frameDuration = 0.1f;
    const int frameW = 14;
    const int frameH = 14;

    enum Direction { RIGHT = 0, LEFT, UP, DOWN };
    Direction faceDir = RIGHT;

    int tick=0;
    int tickCount_x = 0;
    int tickCount_y = 0;
    int cold=100;
    
};
#endif // GHOST_HPP