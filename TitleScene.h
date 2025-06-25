#pragma once
#include "IScene.h"
#include "SceneDefine.h"
#include <Windows.h>

class TitleScene : public IScene {
public:
    void Initialize() override;
    void Update(char* keys, char* preKeys) override;
    void Draw() override;
    bool IsEnd() const override;
    int NextScene() const override;

private:
    float easeOutBounce(float x);

    float posX1_, stPosX1_, edPosX1_;
    float frameX_, endFrameX_;
    bool isMoveX_;

    int color_;
    int alpha_;
    DWORD startTime_;
    DWORD timeCount_;

    int spaceGH_, toGH_, startGH_;

    int nextScene_;
    bool isEnd_;
};
