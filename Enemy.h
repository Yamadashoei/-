#pragma once
#include "EnemyBullet.h"
#include "Particle.h"
#include <vector>

class Enemy {
public:
    Enemy() { Initialize(); }

    void Initialize();
    void Update(float deltaTime);
    void FireBullet();
    void OnHit(std::vector<Particle>& particles);
    void Draw() const;

    float GetX() const;
    float GetY() const;
    float GetW() const;
    float GetH() const;
    int GetLives() const;
    bool IsAlive() const;
    bool IsDamaged() const;
    float GetDamageTimer() const;
    EnemyBullet& GetBullet();

private:
    float posX_;
    float posY_;
    float width_;
    float height_;
    float speed_;

    int maxLives_;
    int lives_;
    bool isAlive_;
    bool isDamaged_;
    float damageTimer_;

    EnemyBullet bullet_;

    // 横移動制御
    float directionX_;
    float moveTimer_;
    float moveInterval_;

    // ジャンプ制御
    bool isJumping_;
    float velocityY_;
    float gravity_;
    float jumpInterval_;
    float jumpTimer_;
};
