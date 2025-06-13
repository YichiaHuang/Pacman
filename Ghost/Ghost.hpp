#ifndef GHOST_HPP
#define GHOST_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/IObject.hpp"

/*class Ghost : public Engine::IObject{
public:
    Ghost(float x, float y);
    virtual ~Ghost();
    virtual void Update(float deltaTime) override;
    virtual void Draw() const override;
    void setDir();
    void escape();
    void reverse();
    void setPacmanPos(const Engine::Point &pos);
    //virtual const char* GetSpriteName() const = 0;
    Engine::Point GetPosition() const { return Position; }
    int animationDirection = 1;

    bool caughtPacman = false;
    bool pause_mode = false;
    float Speed;
    float x, y;
    float frightenedTimer = 0;
    ALLEGRO_BITMAP* spriteSheet = nullptr;
    ALLEGRO_BITMAP* normalSprite = nullptr;
    int tickCount_x = 0;
    int tickCount_y = 0;
    void Reset();
protected:
    Engine::Point Position;
    Engine::Point targetPos;
    Engine::Point pacmanPos;
    int moveDirX, moveDirY;
    int gridX, gridY;

    int animationFrame = 0;
    float animationTimer = 0;
    const int totalFrames = 2;
    const float frameDuration = 0.1f;
    const int frameW = 14;
    const int frameH = 14;

    enum Direction { RIGHT = 0, LEFT, UP, DOWN };
    Direction faceDir = RIGHT;

    int tick=0;
    int coldown = 200;
    bool isFrightened = false;
    
};*/

class Ghost : public Engine::IObject{
public:
    Ghost(float x, float y);
    virtual ~Ghost();
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
protected:
    
    float x, y;
    Engine::Point Position;
    Engine::Point targetPos;
    Engine::Point pacmanPos;
    int moveDirX, moveDirY;
    int gridX, gridY;
    
    ALLEGRO_BITMAP* spriteSheet = nullptr;
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