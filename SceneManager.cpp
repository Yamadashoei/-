#include "SceneManager.h"

SceneManager::SceneManager() : currentScene_(nullptr) {}

SceneManager::~SceneManager() {
    if (currentScene_) {
        delete currentScene_;
    }
}

void SceneManager::Initialize() {
    if (currentScene_) {
        delete currentScene_;
    }
    currentScene_ = new TitleScene();
    currentScene_->Initialize();
}

void SceneManager::Update(char* keys, char* preKeys) {
    if (!currentScene_) return;

    currentScene_->Update(keys, preKeys);

    if (currentScene_->IsEnd()) {
        int next = currentScene_->NextScene();

        delete currentScene_;
        currentScene_ = nullptr;

        switch (next) {
        case SCENE_TITLE:
            currentScene_ = new TitleScene();
            break;
        case SCENE_GAME:
            currentScene_ = new Game();
            break;
        case SCENE_CLEAR:
            currentScene_ = new ClearScene();
            break;
        }

        if (currentScene_) {
            currentScene_->Initialize();
        }
    }
}

void SceneManager::Draw() {
    if (currentScene_) {
        currentScene_->Draw();
    }
}
