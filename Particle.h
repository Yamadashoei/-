#pragma once

class Particle {
public:
    Particle(float x, float y, float vx, float vy, float lifetime);

    void Update();
    void Draw() const;
    bool IsDead() const;

private:
    float x_;
    float y_;
    float vx_;
    float vy_;
    float lifetime_;
};
