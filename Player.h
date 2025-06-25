#pragma once
#include <Vector2.h>
#include "PlayerBullet.h"

class Player {
public:
    Player();
    void Initialize();
    void Update(char* keys, char* preKeys);
    void FireBullet(char* keys, char* preKeys);
    void Draw() const;

    float GetX() const { return posX_; }
    float GetY() const { return posY_; }
    float GetW() const { return width_; }
    float GetH() const { return height_; }
    PlayerBullet& GetBullet() { return bullet_; }

private:
    float posX_;
    float posY_;
    float moveSpeed_;
    float width_;
    float height_;
    Vector2 velocity_;
    bool isJumping_;
    bool isFacingRight_;

    PlayerBullet bullet_;
};
