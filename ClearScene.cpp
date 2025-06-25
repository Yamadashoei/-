#include "ClearScene.h"
#include <Novice.h>

void ClearScene::Initialize() {
    isEnd_ = false;
    nextScene_ = SCENE_TITLE;
}

void ClearScene::Update(char* keys, char* preKeys) {
    if (keys[DIK_R] && !preKeys[DIK_R]) {
        isEnd_ = true;
    }
}

void ClearScene::Draw() {
    Novice::DrawBox(0, 0, 640, 360, 0.0f, 0x000000FF, kFillModeSolid);
    Novice::ScreenPrintf(200, 160, "CLEAR! PRESS R TO RETRY");
}

bool ClearScene::IsEnd() const {
    return isEnd_;
}

int ClearScene::NextScene() const {
    return nextScene_;
}
