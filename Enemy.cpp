#include "Enemy.h"
#include <Novice.h>
#include <cstdlib>

void Enemy::Initialize() {
    posX_ = 400.0f;
    posY_ = 240.0f;
    width_ = 20.0f;
    height_ = 20.0f;
    speed_ = 3.0f;
    maxLives_ = 5;
    lives_ = 5;
    isAlive_ = true;
    isDamaged_ = false;
    damageTimer_ = 0.0f;
    bullet_ = EnemyBullet();

    directionX_ = (rand() % 2 == 0) ? -1.0f : 1.0f;
    moveTimer_ = 0.0f;
    moveInterval_ = 1.0f + static_cast<float>(rand() % 200) / 100.0f;

    isJumping_ = false;
    velocityY_ = 0.0f;
    gravity_ = 0.5f;
    jumpInterval_ = 2.0f + static_cast<float>(rand() % 100) / 50.0f;
    jumpTimer_ = 0.0f;
}

void Enemy::Update(float deltaTime) {
    if (isDamaged_) {
        damageTimer_ -= deltaTime;
        if (damageTimer_ <= 0.0f) {
            isDamaged_ = false;
        }
    }

    bullet_.Update();

    // ランダム横移動
    moveTimer_ += deltaTime;
    if (moveTimer_ >= moveInterval_) {
        moveTimer_ = 0.0f;
        moveInterval_ = 1.0f + static_cast<float>(rand() % 200) / 100.0f;
        directionX_ = (rand() % 2 == 0) ? -1.0f : 1.0f;
    }

    posX_ += directionX_ * speed_;
    if (posX_ < 0.0f) {
        posX_ = 0.0f;
        directionX_ = 1.0f;
    }
    else if (posX_ + width_ > 640.0f) {
        posX_ = 640.0f - width_;
        directionX_ = -1.0f;
    }

    // ジャンプ処理
    jumpTimer_ += deltaTime;
    if (jumpTimer_ >= jumpInterval_ && !isJumping_) {
        isJumping_ = true;
        velocityY_ = -10.0f;
        jumpTimer_ = 0.0f;
        jumpInterval_ = 2.0f + static_cast<float>(rand() % 100) / 50.0f;
    }

    if (isJumping_) {
        posY_ += velocityY_;
        velocityY_ += gravity_;
        if (posY_ >= 240.0f) {
            posY_ = 240.0f;
            isJumping_ = false;
            velocityY_ = 0.0f;
        }
    }
}

void Enemy::FireBullet() {
    if (!bullet_.IsActive()) {
        bullet_.Initialize(posX_, posY_, -5.0f);
    }
}

void Enemy::OnHit(std::vector<Particle>& particles) {
    lives_--;
    isDamaged_ = true;
    damageTimer_ = 0.5f;

    if (lives_ <= 0 && isAlive_) {
        isAlive_ = false;

        const int kMaxParticles = 30;
        for (int i = 0; i < kMaxParticles; ++i) {
            float vx = static_cast<float>((rand() % 100) - 50) / 20.0f;
            float vy = static_cast<float>((rand() % 100) - 50) / 20.0f;
            particles.emplace_back(posX_, posY_, vx, vy, 3.0f);
        }
    }
}

void Enemy::Draw() const {
    if (isAlive_) {
        unsigned int color = 0x000000FF;
        if (isDamaged_ && static_cast<int>(damageTimer_ * 10) % 2 == 0) {
            color = 0xFF0000FF;
        }

        Novice::DrawBox(
            static_cast<int>(posX_), static_cast<int>(posY_),
            static_cast<int>(width_), static_cast<int>(height_),
            0.0f, color, kFillModeSolid);
    }

    bullet_.Draw();
}

float Enemy::GetX() const { return posX_; }
float Enemy::GetY() const { return posY_; }
float Enemy::GetW() const { return width_; }
float Enemy::GetH() const { return height_; }
int Enemy::GetLives() const { return lives_; }
bool Enemy::IsAlive() const { return isAlive_; }
bool Enemy::IsDamaged() const { return isDamaged_; }
float Enemy::GetDamageTimer() const { return damageTimer_; }
EnemyBullet& Enemy::GetBullet() { return bullet_; }
