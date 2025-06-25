#pragma once

#include "IScene.h"
#include "SceneDefine.h"
#include "TitleScene.h"
#include "Game.h"
#include "ClearScene.h"

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void Initialize();
    void Update(char* keys, char* preKeys);
    void Draw();

private:
    IScene* currentScene_;
};
