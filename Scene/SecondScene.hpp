#ifndef SECONDSCENE_HPP
#define SECONDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Pacman/Pac.hpp"    
#include "Slot/SlotMachine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "GhostSecond/GhostSecond.hpp"
class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class SecondScene final : public Engine::IScene {
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
    static ALLEGRO_BITMAP* frightenedBitmap;
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
    explicit SecondScene() = default;
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
 
    
    int map_dot[40][40]={0};
    // void ModifyReadMapTiles();
    Pac* player;
    Engine::Point cameraPos = Engine::Point(0, 0);
    int total_dot;
    void SlotOnClick(int stage);
    SlotMachine* slotMachine;
    bool slot_mode=false;
    bool slot_yet=true;
    GhostSecond* ghost[4];
    int red_coldown = 0;
    int pause_coldown = 0;
    int random[14];
    ALLEGRO_BITMAP* miniMapBitmap = nullptr;
    const int MiniMapSize = 150; // 小地圖的像素寬高（正方形）
    void GenerateMiniMap();
    void UpdateMiniMapCell(int i, int j);
    float remainingTime; // 剩餘時間（秒）
    Engine::Label* timerLabel; // 顯示用的UI Label
    void ReplayOnClick();
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    bool replayRequested = false;
    int freeze_coldown;
    bool freeze_mode=false;

};
#endif   // PLAYSCENE_HPP
