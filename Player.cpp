#include "Player.h"
#include <Novice.h>

Player::Player() {
    Initialize();
}

void Player::Initialize() {
    posX_ = 100.0f;
    posY_ = 245.0f;
    moveSpeed_ = 3.0f;
    width_ = 15.0f;
    height_ = 15.0f;
    velocity_ = { 0.0f, 0.0f };
    isJumping_ = false;
    isFacingRight_ = true;
    bullet_ = PlayerBullet();
}

void Player::Update(char* keys, char* preKeys) {
    if (keys[DIK_A]) {
        posX_ -= moveSpeed_;
        isFacingRight_ = false;
    }

    if (keys[DIK_D]) {
        posX_ += moveSpeed_;
        isFacingRight_ = true;
    }

    if (keys[DIK_W] && !preKeys[DIK_W] && !isJumping_) {
        isJumping_ = true;
        velocity_.y = -10.0f;
    }

    if (isJumping_) {
        posY_ += velocity_.y;
        velocity_.y += 0.5f;

        if (posY_ >= 245.0f) {
            posY_ = 245.0f;
            isJumping_ = false;
            velocity_.y = 0.0f;
        }
    }
    else {
        posY_ += 5.0f;
        if (posY_ >= 245.0f) {
            posY_ = 245.0f;
        }
    }

    bullet_.Update();
}

void Player::FireBullet(char* keys, char* preKeys) {
    if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
        if (!bullet_.IsActive()) {
            float speed = isFacingRight_ ? 5.0f : -5.0f;
            bullet_.Initialize(posX_, posY_, speed);
        }
    }
}

void Player::Draw() const {
    Novice::DrawBox(
        static_cast<int>(posX_),
        static_cast<int>(posY_),
        static_cast<int>(width_),
        static_cast<int>(height_),
        0.0f,
        0xFFFFFFFF,
        kFillModeSolid);

    bullet_.Draw();
}
