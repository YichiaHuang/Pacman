#ifndef SHOVELTOOL_HPP
#define SHOVELTOOL_HPP
#include "Tool.hpp"
#include "Scene/PlayScene.hpp"

class ShovelTool : public Tool {
private:
    PlayScene* scene;
public:
    explicit ShovelTool(PlayScene* scene);
    void Use(int x, int y) override;
    const char* GetName() const override;
};

#endif
