#include "TitleScene.h"
#include <Novice.h>
#include <cmath>
#include <ctime>

void TitleScene::Initialize() {
    color_ = 0xffffff64;
    alpha_ = 0x00000001;
    startTime_ = timeGetTime();
    timeCount_ = 4000;
    frameX_ = 0.0f;
    endFrameX_ = 100.0f;
    isMoveX_ = false;
    stPosX1_ = 0.0f;
    edPosX1_ = 640.0f;
    posX1_ = 0.0f;
    nextScene_ = SCENE_TITLE;
    isEnd_ = false;

    spaceGH_ = Novice::LoadTexture("./resources/Images/SPACE.png");
    toGH_ = Novice::LoadTexture("./resources/Images/TO.png");
    startGH_ = Novice::LoadTexture("./resources/Images/START.png");
}

void TitleScene::Update(char* keys, char* preKeys) {
    DWORD elapsedTime = timeGetTime() - startTime_;

    if (elapsedTime < timeCount_ / 2) {
        if (color_ > 0xffffff01) {
            color_ -= alpha_;
        }
    }
    else {
        if (color_ < 0xffffffff) {
            color_ += alpha_;
        }
    }

    if (elapsedTime > timeCount_) {
        startTime_ = timeGetTime();
    }

    if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
        isMoveX_ = true;
        frameX_ = 0;
    }

    if (isMoveX_) {
        frameX_++;
        if (frameX_ >= endFrameX_) {
            isMoveX_ = false;
            isEnd_ = true;
            nextScene_ = SCENE_GAME;
        }
    }

    float x = frameX_ / endFrameX_;
    posX1_ = stPosX1_ + (edPosX1_ - stPosX1_) * easeOutBounce(x);
}

void TitleScene::Draw() {
    Novice::DrawBox(static_cast<int>(posX1_), 0, 640, 360, 0.0f, 0x0a0a66ff, kFillModeSolid);
    Novice::DrawSprite(200, 240, spaceGH_, 1.0f, 1.0f, 0.0f, color_ | (alpha_ << 24));
    Novice::DrawSprite(290, 240, toGH_, 1.0f, 1.0f, 0.0f, color_ | (alpha_ << 24));
    Novice::DrawSprite(390, 240, startGH_, 1.0f, 1.0f, 0.0f, color_ | (alpha_ << 24));
    Novice::ScreenPrintf(12, 24, "SPACE:MODE");
}

bool TitleScene::IsEnd() const {
    return isEnd_;
}

int TitleScene::NextScene() const {
    return nextScene_;
}

float TitleScene::easeOutBounce(float x) {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (x < 1 / d1) {
        return n1 * x * x;
    }
    else if (x < 2 / d1) {
        x -= 1.5f / d1;
        return n1 * x * x + 0.75f;
    }
    else if (x < 2.5f / d1) {
        x -= 2.25f / d1;
        return n1 * x * x + 0.9375f;
    }
    else {
        x -= 2.625f / d1;
        return n1 * x * x + 0.984375f;
    }
}
