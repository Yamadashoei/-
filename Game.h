#pragma once
#include "IScene.h"
#include "SceneDefine.h"
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"
#include <vector>

class Game : public IScene {
public:
    void Initialize() override;
    void Update(char* keys, char* preKeys) override;
    void Draw() override;
    bool IsEnd() const override;
    int NextScene() const override;

private:
    Player player_;
    Enemy enemy_;
    std::vector<Particle> particles_;

    bool isEnd_;
    int nextScene_;

    // 敵の弾がプレイヤーに当たったときの処理
    bool isPlayerDamaged_;
    float playerDamageTimer_;
};
