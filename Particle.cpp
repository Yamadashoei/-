#include "Particle.h"
#include <Novice.h>

Particle::Particle(float x, float y, float vx, float vy, float lifetime)
    : x_(x), y_(y), vx_(vx), vy_(vy), lifetime_(lifetime) {
}

void Particle::Update() {
    x_ += vx_;
    y_ += vy_;
    lifetime_ -= 0.05f;
}

void Particle::Draw() const {
    if (lifetime_ > 0.0f) {
        Novice::DrawEllipse(
            static_cast<int>(x_), static_cast<int>(y_),
            3, 3, 0.0f, 0xFF0000FF, kFillModeSolid);
    }
}

bool Particle::IsDead() const {
    return lifetime_ <= 0.0f;
}
