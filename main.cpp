#include <Novice.h>
#include "SceneManager.h"

const char kWindowTitle[] = "SceneSystemGame";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 640, 360);

    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    SceneManager sceneManager;
    sceneManager.Initialize();

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        sceneManager.Update(keys, preKeys);
        sceneManager.Draw();

        Novice::EndFrame();

        if (keys[DIK_ESCAPE]) {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}
