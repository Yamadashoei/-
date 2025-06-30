#include <Novice.h>
#include <Vector2.h>
#include <list>

const char kWindowTitle[] = "2Dシューティング";

enum Scene {
    Scene_Title,
    Scene_Game,
    Scene_GameOver,
    Scene_Clear
};

struct Bullet {
    Vector2 pos;
    Vector2 velocity;
    bool isEnemy;
};

struct Player {
    Vector2 pos;
    int texture;
};

struct Enemy {
    Vector2 pos;
    int texture;
    int hp;
    float speedX;
    bool isDamaged;
    int damageTimer;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    int texTitle = Novice::LoadTexture("./Resources/Images/AL_title.png");
    int texBG = Novice::LoadTexture("./Resources/Images/bg.png");
    int texPlayer = Novice::LoadTexture("./Resources/Images/AL_player.png");
    int texEnemy = Novice::LoadTexture("./Resources/Images/AL_enemy.png");
    int texBullet = Novice::LoadTexture("./Resources/Images/AL_player_bullet.png");
    int texEnemyBullet = Novice::LoadTexture("./Resources/Images/AL_enemy_bullet.png");
    int texGameOver = Novice::LoadTexture("./Resources/Images/AL_gameOver.png");
    int texClear = Novice::LoadTexture("./Resources/Images/AL_CLEAR.png");
    int texEarth = Novice::LoadTexture("./Resources/Images/AL_earth.png");
    int texExplosion = Novice::LoadTexture("./Resources/Images/AL_boss_explosion.png");

    int titleBgmHandle = Novice::LoadAudio("./Resources/Sounds/danceSpace.mp3");
    int gameBgmHandle = Novice::LoadAudio("./Resources/Sounds/shotSpace.mp3");
    int titleBgmInstance = -1;
    int gameBgmInstance = -1;

    Scene scene = Scene_Title;
    Scene prevScene = Scene_Title;

    const int moveAreaTop = 300;
    const int moveAreaBottom = 680;

    Player player;
    Enemy enemy;
    std::list<Bullet> bullets;
    float enemyShotTimer = 0.0f;
    bool showExplosion = false;
    int explosionTimer = 0;

    auto ResetGame = [&]() {
        player = { {640, 600}, texPlayer };
        enemy = { {640, 100}, texEnemy, 100, 5.0f, false, 0 };
        bullets.clear();
        enemyShotTimer = 0.0f;
        showExplosion = false;
        explosionTimer = 0;
        };

    ResetGame();

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        if (scene != prevScene || (scene == Scene_Title && titleBgmInstance == -1)) {
            if (titleBgmInstance != -1) {
                Novice::StopAudio(titleBgmInstance);
                titleBgmInstance = -1;
            }
            if (gameBgmInstance != -1) {
                Novice::StopAudio(gameBgmInstance);
                gameBgmInstance = -1;
            }
            if (scene == Scene_Game) {
                gameBgmInstance = Novice::PlayAudio(gameBgmHandle, true, 0.3f);
            }
            else {
                titleBgmInstance = Novice::PlayAudio(titleBgmHandle, true, 0.3f);
            }
            prevScene = scene;
        }

        switch (scene) {
        case Scene_Title:
            Novice::DrawSprite(0, 0, texBG, 1, 1, 0.0f, 0xFFFFFFFF);
            Novice::DrawSprite(0, 0, texTitle, 1, 1, 0.0f, 0xFFFFFFFF);
            if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN]) {
                ResetGame();
                scene = Scene_Game;
            }
            break;

        case Scene_Game: {
            Novice::DrawSprite(0, 0, texBG, 1, 1, 0.0f, 0xFFFFFFFF);
            int earthX = 0;
            int earthY = 620;
            Novice::DrawSprite(earthX, earthY, texEarth, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

            if (keys[DIK_W]) player.pos.y -= 5;
            if (keys[DIK_S]) player.pos.y += 5;
            if (keys[DIK_A]) player.pos.x -= 5;
            if (keys[DIK_D]) player.pos.x += 5;

            if (player.pos.y < moveAreaTop) player.pos.y = moveAreaTop;
            if (player.pos.y > moveAreaBottom) player.pos.y = moveAreaBottom;
            if (player.pos.x < 32) player.pos.x = 32;
            if (player.pos.x > 1248) player.pos.x = 1248;

            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE]) {
                bullets.push_back({ player.pos, {0, -10}, false });
            }

            enemy.pos.x += enemy.speedX;
            if (enemy.pos.x <= 96 || enemy.pos.x >= 1280 - 96) {
                enemy.speedX *= -1;
            }

            enemyShotTimer += 1.0f / 30.0f;
            if (enemyShotTimer >= 3.0f) {
                bullets.push_back({ {enemy.pos.x, enemy.pos.y + 64}, {0, 10}, true });
                enemyShotTimer = 0.0f;
            }

            for (auto it = bullets.begin(); it != bullets.end();) {
                it->pos.x += it->velocity.x;
                it->pos.y += it->velocity.y;

                if (!it->isEnemy) {
                    float dx = enemy.pos.x - it->pos.x;
                    float dy = enemy.pos.y - it->pos.y;
                    float distSq = dx * dx + dy * dy;
                    if (distSq < 64 * 64 && enemy.hp > 0) {
                        enemy.hp -= 5;
                        enemy.isDamaged = true;
                        enemy.damageTimer = 18;
                        it = bullets.erase(it);
                        if (enemy.hp <= 0) {
                            showExplosion = true;
                            explosionTimer = 60;
                            scene = Scene_Clear;
                        }
                        continue;
                    }
                }
                else {
                    float dx = player.pos.x - it->pos.x;
                    float dy = player.pos.y - it->pos.y;
                    float distSq = dx * dx + dy * dy;
                    if (distSq < 128 * 128) {
                        scene = Scene_GameOver;
                        it = bullets.erase(it);
                        continue;
                    }
                }


                if (it->pos.y < -50 || it->pos.y > 800) {
                    it = bullets.erase(it);
                }
                else {
                    ++it;
                }
            }

            if (enemy.hp > 0) {
                if (!enemy.isDamaged || (enemy.damageTimer / 3) % 2 == 0) {
                    Novice::DrawSprite((int)(enemy.pos.x - 96), (int)(enemy.pos.y - 96), enemy.texture, 3.0f, 3.0f, 0.0f, 0xFFFFFFFF);
                }
            }

            if (enemy.isDamaged) {
                enemy.damageTimer--;
                if (enemy.damageTimer <= 0) {
                    enemy.isDamaged = false;
                }
            }

            if (showExplosion) {
                if (explosionTimer > 0) {
                    Novice::DrawSprite((int)(enemy.pos.x - 96), (int)(enemy.pos.y - 96), texExplosion, 3.0f, 3.0f, 0.0f, 0xFFFFFFFF);
                    explosionTimer--;
                }
                else {
                    showExplosion = false;
                }
            }

            for (auto& b : bullets) {
                int tex = b.isEnemy ? texEnemyBullet : texBullet;
                Novice::DrawSprite((int)(b.pos.x - 8), (int)(b.pos.y - 8), tex, 1, 1, 0.0f, 0xFFFFFFFF);
            }

            Novice::DrawSprite((int)(player.pos.x - 32), (int)(player.pos.y - 32), player.texture, 1, 1, 0.0f, 0xFFFFFFFF);

            Novice::DrawLine(0, moveAreaTop, 1280, moveAreaTop, 0xFF0000FF);
            //Novice::DrawLine(0, moveAreaBottom, 1280, moveAreaBottom, 0xFF0000FF);

            unsigned int hpColor = 0x00FF00FF;
            if (enemy.hp <= 80 && enemy.hp > 30) hpColor = 0x0000FFFF;
            if (enemy.hp <= 30) hpColor = 0xFF0000FF;
            int hpHeight = (int)(300.0f * (enemy.hp / 100.0f));
            int barY = 50 + (300 - hpHeight);
            Novice::DrawBox(50, 50, 20, 300, 0.0f, 0x444444FF, kFillModeSolid);
            Novice::DrawBox(50, barY, 20, hpHeight, 0.0f, hpColor, kFillModeSolid);

            break;
        }

        case Scene_GameOver:
            Novice::DrawSprite(0, 0, texBG, 1, 1, 0.0f, 0xFFFFFFFF);
            Novice::DrawSprite(0, 0, texGameOver, 1, 1, 0.0f, 0xFFFFFFFF);
            if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN]) {
                scene = Scene_Title;
            }
            break;

        case Scene_Clear:
            Novice::DrawSprite(0, 0, texBG, 1, 1, 0.0f, 0xFFFFFFFF);
            Novice::DrawSprite(0, 0, texClear, 1, 1, 0.0f, 0xFFFFFFFF);
            if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN]) {
                scene = Scene_Title;
            }
            break;
        }

        Novice::EndFrame();
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE]) break;
    }

    Novice::Finalize();
    return 0;
}
