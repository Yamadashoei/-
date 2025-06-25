#pragma once
#include "IScene.h"
#include "SceneDefine.h"

class ClearScene : public IScene {
public:
    void Initialize() override;
    void Update(char* keys, char* preKeys) override;
    void Draw() override;
    bool IsEnd() const override;
    int NextScene() const override;

private:
    bool isEnd_;
    int nextScene_;
};
