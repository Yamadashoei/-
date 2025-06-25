#include "PlayerBullet.h"
#include <Novice.h>

PlayerBullet::PlayerBullet()
    : x_(0.0f), y_(0.0f), speed_(0.0f), radius_(3.0f), isActive_(false) {
}

void PlayerBullet::Initialize(float startX, float startY, float speed) {
    x_ = startX;
    y_ = startY;
    speed_ = speed;
    isActive_ = true;
}

void PlayerBullet::Update() {
    if (!isActive_) return;

    x_ += speed_;

    if (x_ < 0.0f || x_ > 640.0f) {
        isActive_ = false;
    }
}

void PlayerBullet::Draw() const {
    if (!isActive_) return;

    Novice::DrawEllipse(
        static_cast<int>(x_),
        static_cast<int>(y_),
        static_cast<int>(radius_),
        static_cast<int>(radius_),
        0.0f,
        0xFF0000FF, // 赤
        kFillModeSolid);
}
