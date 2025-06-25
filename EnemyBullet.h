#pragma once

class EnemyBullet {
public:
    EnemyBullet();
    void Initialize(float startX, float startY, float speed);
    void Update();
    void Draw() const;

    float GetX() const { return x_; }
    float GetY() const { return y_; }
    float GetRadius() const { return radius_; }
    bool IsActive() const { return isActive_; }
    void Deactivate() { isActive_ = false; }

private:
    float x_;
    float y_;
    float speed_;
    float radius_;
    bool isActive_;
};
