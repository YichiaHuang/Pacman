#include "ShovelTool.hpp"
#include "Turret/Turret.hpp"
#include <cmath>

ShovelTool::ShovelTool(PlayScene* scene) : scene(scene) {}

void ShovelTool::Use(int x, int y) {
    // 將格子座標轉換成像素中心座標
    int cx = x * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    int cy = y * PlayScene::BlockSize + PlayScene::BlockSize / 2;

    // 直接操作 TowerGroup 內的物件
    for (auto it = scene->TowerGroup->GetObjects().begin();
              it != scene->TowerGroup->GetObjects().end(); ++it) {
        Turret* turret = dynamic_cast<Turret*>(*it);
        if (!turret) continue;

        int tx = turret->Position.x / PlayScene::BlockSize;
        int ty = turret->Position.y / PlayScene::BlockSize;

        if (tx == x && ty == y) {
            // 移除砲塔物件
            scene->TowerGroup->RemoveObject(turret->GetObjectIterator());
            // 將地圖狀態設回空地
            scene->mapState[y][x] = PlayScene::TILE_DIRT;
            break;
        }
    }
}

const char* ShovelTool::GetName() const {
    return "Shovel";
}
