#include "Game.h"
#include <Novice.h>
#include <algorithm>
#include <Particle.cpp>

void Game::Initialize() {
    player_.Initialize();
    enemy_.Initialize();
    particles_.clear();
    isEnd_ = false;
    nextScene_ = SCENE_GAME;

    isPlayerDamaged_ = false;
    playerDamageTimer_ = 0.0f;
}

void Game::Update(char* keys, char* preKeys) {
    player_.Update(keys, preKeys);
    player_.FireBullet(keys, preKeys);

    enemy_.Update(0.016f);
    enemy_.FireBullet();

    // プレイヤー弾 vs 敵
    if (player_.GetBullet().IsActive() && enemy_.IsAlive()) {
        float bx = player_.GetBullet().GetX();
        float by = player_.GetBullet().GetY();
        float br = player_.GetBullet().GetRadius();
        float ex = enemy_.GetX();
        float ey = enemy_.GetY();
        float ew = enemy_.GetW();
        float eh = enemy_.GetH();

        if (bx < ex + ew && bx + br > ex && by < ey + eh && by + br > ey) {
            player_.GetBullet().Deactivate();
            enemy_.OnHit(particles_);
        }
    }

    // 敵弾 vs プレイヤー
    EnemyBullet& eBullet = enemy_.GetBullet();
    if (eBullet.IsActive()) {
        float bx = eBullet.GetX();
        float by = eBullet.GetY();
        float br = eBullet.GetRadius();

        float px = player_.GetX();
        float py = player_.GetY();
        float pw = player_.GetW();
        float ph = player_.GetH();

        if (bx < px + pw && bx + br > px &&
            by < py + ph && by + br > py &&
            !isPlayerDamaged_) {
            isPlayerDamaged_ = true;
            playerDamageTimer_ = 0.5f;
            eBullet.Deactivate();
        }
    }

    // プレイヤーダメージ無敵時間処理
    if (isPlayerDamaged_) {
        playerDamageTimer_ -= 0.016f;
        if (playerDamageTimer_ <= 0.0f) {
            isPlayerDamaged_ = false;
        }
    }

    for (auto& p : particles_) {
        p.Update();
    }
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
            [](const Particle& p) { return p.IsDead(); }),
        particles_.end());

    if (!enemy_.IsAlive() && particles_.empty()) {
        isEnd_ = true;
        nextScene_ = SCENE_CLEAR;
    }
}

void Game::Draw() {
    Novice::DrawBox(0, 0, 640, 360, 0.0f, 0xBDBDBDFF, kFillModeSolid);
    Novice::DrawBox(0, 260, 640, 100, 0.0f, 0xA9E2F3FF, kFillModeSolid);

    if (!(isPlayerDamaged_ && static_cast<int>(playerDamageTimer_ * 10) % 2 == 0)) {
        player_.Draw();  // 点滅中は非表示にして点滅表現
    }

    enemy_.Draw();

    for (const auto& p : particle
