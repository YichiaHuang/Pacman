#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Pacman/Pacman.hpp"    


class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
    bool WinTriggered = false;
    bool opening = true;
    float openingTimer = 0;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> startSound;
    Engine::Label* playerOneLabel = nullptr;
    Engine::Label* readyLabel = nullptr;
    bool paused = false;
    Engine::Label* pauseLabel = nullptr;

protected:
    int lives;
    int money;
    int SpeedMult;
    std::set<int> keyPressed; 
public:
    enum TileType {
            TILE_DIRT,
            TILE_FLOOR,
            TILE_OCCUPIED,
        };
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;


    Group *DotsGroup;

    Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UILives;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    Turret *preview;
    Engine::Image* shovelPreview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    //void OnMouseDown(int button, int mx, int my) override;
    //void OnMouseMove(int mx, int my) override;
    //void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Hit();
    int GetMoney() const{return money;}
    //void EarnMoney(int money);
    void ReadMap();
    //void ReadEnemyWave();
    void ConstructUI();
    //void UIBtnClicked(int id);
    //bool CheckSpaceValid(int x, int y);
    //std::vector<std::vector<int>> CalculateBFSDistance();
 
    
    int map_dot[30][30]={0};
    // void ModifyReadMapTiles();
    Pacman* player;
    int total_dot;
  
    
};
#endif   // PLAYSCENE_HPP
