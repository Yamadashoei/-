#include <Novice.h>
#include <Vector2.h>
#include <cmath>
#include<math.h>
#include <random>
#include <corecrt_math.h>

const char kWindowTitle[] = "LC1C_27_ヤマダ_ショウエイ";

struct PLAYER {
	float posX;
	float posY;
	float move;
	float w;
	float h;

	Vector2 v;

	bool isAlive; //
	bool isJumping; //
	bool isFacingRight; // プレイヤーの向き
};

//敵の初期
struct ENEMY {
	float posX;
	float posY;
	float move;
	float w;
	float h;

	int maxLives;  //敵の最大HP
	int lives;  // 敵のライフ
	bool isAlive;
	bool isDamaged;  //敵がダメージを受けているか
	float damageTimer;

};

//弾と初期
struct BULLET {
	float posX;// 弾のX位置
	float posY;// 弾のY位置
	float speed;// 弾の速度
	float radius;//弾の半径

	bool isBulletFlag;// 弾のフラグ
};

const int kMaxParticles = 30;

//パーティクル(爆発)の初期化
struct Particle {
	float x, y;      // パーティクルの位置
	float vx, vy;    // パーティクルの速度
	float lifetime;  //パーティクルの時間
};

// 当たり判定関数
bool CheckCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	return x1 < x2 + w2 &&
		x1 + w1 > x2 &&
		y1 < y2 + h2 &&
		y1 + h1 > y2;
}

// パーティクル生成関数
void CreateParticles(ENEMY& enemy, std::vector<Particle>& particles) {
	for (int i = 0; i < kMaxParticles; ++i) {
		Particle particle;
		particle.x = enemy.posX;
		particle.y = enemy.posY;
		particle.vx = static_cast<float>((rand() % 100) - 50) / 20.0f;
		particle.vy = static_cast<float>((rand() % 100) - 50) / 20.0f;
		particle.lifetime = static_cast<float>(rand() % 30) / 10.0f;
		particle.lifetime = 3.0f;  // すべてのパーティクルを3秒後に停止させる
		particles.push_back(particle);
	}
}

// シーン切り替えの情報
enum SCENENO {
	TITLE,
	MODE,
	GAME,
	CLEAR
};


struct Snow {
	float x, y;  // 座標
	float speedX, speedY;  // 速度
};

const int MaxSnowP = 100;  // パーティクルの数
Snow snowy[MaxSnowP];      // 雪の粒子の配列

// 初期化関数
void InitializeSnow() {
	for (int i = 0; i < MaxSnowP; ++i) {
		snowy[i].x = static_cast<float>(rand() % 1280);
		snowy[i].y = static_cast<float>(rand() % 720);
		snowy[i].speedX = static_cast<float>(rand() % 5 - 2);  // X方向の速さ
		snowy[i].speedY = static_cast<float>(rand() % 3 + 1);  // Y方向の速さ
	}
}

// 更新関数
void UpdateSnow() {
	for (int i = 0; i < MaxSnowP; ++i) {
		snowy[i].x += snowy[i].speedX;
		snowy[i].y += snowy[i].speedY;

		// 画面外に出たら再配置
		if (snowy[i].x < 0 ||
			snowy[i].x > 1280 ||
			snowy[i].y > 720) {

			snowy[i].x = static_cast<float>(rand() % 1280);
			snowy[i].y = 0;
			snowy[i].speedX = static_cast<float>(rand() % 5 - 2);
			snowy[i].speedY = static_cast<float>(rand() % 3 + 1);
		}
	}
}

// 描画関数
void DrawSnow() {
	for (int i = 0; i < MaxSnowP; ++i) {
		Novice::DrawEllipse(
			static_cast<int>(snowy[i].x),
			static_cast<int>(snowy[i].y),
			3, 3,
			0.0f, 0xCEF6F5FF, kFillModeSolid);  // 白色の雪
	}
}

float easeOutBounce(float x) { //フェードインの関数
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (x < 1 / d1) {
		return static_cast<float>(n1 * x * x);
	}
	else if (x < 2 / d1) {
		x = static_cast<float>(x - 1.5 / d1);
		return static_cast<float>(n1 * x * x + 0.75);
	}
	else if (x < 2.5 / d1) {
		x = static_cast<float>(x - 2.25 / d1);
		return static_cast<float>(n1 * x * x + 0.9375);
	}
	else {
		x = static_cast<float>(x - 2.625 / d1);
		return static_cast<float>(n1 * x * x + 0.984375);
	}
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 640, 360);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//プレイヤーの初期化
	PLAYER player{
	100.0f,245.0f,
	3.0f,
	15.0f,15.0f,

	 3.0f,
	false,
	false,
	true // 最初は右を向いている
	};

	//敵の初期化
	ENEMY enemy{
	400.0f,240.0f,
	3.0f,
	20.0f,20.0f,
	3,  // 敵の最大HP
	3, // 敵の初期HP
	true,
	false,  // 初期状態ではダメージを受けていない
	0.0f
	};

	bool isParticleEffectActive = true; // 初めは演出がアクティブ

	//弾の初期化
	BULLET bullet{
		0.0f,0.0f,
		5.0f,
		3.0f,

		false
	};

	// パーティクルのリスト
	std::vector<Particle> particles;

	// 現在のシーン
	SCENENO sceneNo = TITLE;

	// 各シーンの座標
	float scenePosX = 0.0f;
	float scenePosY = 0.0f;

	// 文字画像
	int SPACE_GH =
		Novice::LoadTexture("./resources/Images/SPACE.png");
	int TO_GH =
		Novice::LoadTexture("./resources/Images/TO.png");
	int START_GH =
		Novice::LoadTexture("./resources/Images/START.png");


	int color = 0xffffff64;//半透明
	int alpha = 0x00000001;//めっちゃ透明

	DWORD startTime = timeGetTime(); //時間開始
	const DWORD timeCount = 4000; // 4秒間


	//フェードイン演出
	float posX1 = 0.0f;

	float stPosX1 = 0.0f;    //始まりの位置
	float edPosX1 = 640.0f;    //終わりの位置


	float frameX = 0.0f;     //現在の時間
	float endFrameX = 100.0f; //全体の時間

	bool isMoveX = false;

	float x = 100.0f; //ボックスの初期化


	// 雪の初期化
	InitializeSnow();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// シーンごとの処理
		switch (sceneNo) {
		case TITLE:
		{
			DWORD elapsedTime = timeGetTime() - startTime; //時間経過

			if (elapsedTime < timeCount / 2) {
				//時間の経過で徐々に色が薄くなる
				if (color > 0xffffff01) {
					color -= alpha;
				}
			}
			else {
				//時間の経過で徐々に色が濃くなる
				if (color < 0xffffffff) {
					color += alpha;
				}
			}

			if (elapsedTime > timeCount) {
				startTime = timeGetTime();//タイマーのリセット
			}


			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				isMoveX = 1;
				frameX = 0;
			}

			if (isMoveX == true) {
				frameX++;
			}
			if (frameX == endFrameX) {
				isMoveX = 0;
				sceneNo = MODE;
			}

			x = frameX / endFrameX;

			//だんだん早く
			posX1 = stPosX1 + (edPosX1 - stPosX1) * easeOutBounce(x);

			break;
		}
		case MODE:

			// MODEからGAMEへの切り替え
			if (keys[DIK_1] && !preKeys[DIK_1]) {
				sceneNo = GAME;
			}

			break;

		case GAME:
			// GAMEシーンの更新処理
			UpdateSnow();

			if (keys[DIK_RETURN] && !preKeys[DIK_RETURN]) {
				sceneNo = MODE;
			}


			if (keys[DIK_A]) {
				player.posX -= player.move;
				player.isFacingRight = false;
			}

			if (keys[DIK_D]) {
				player.posX += player.move;
				player.isFacingRight = true;
			}

			if (keys[DIK_W] && !preKeys[DIK_W] && !player.isJumping) {
				player.isJumping = true;
				player.v.y = -10.0f;
			}

			if (player.isJumping) {
				player.posY += player.v.y;
				player.v.y += 0.5f;

				if (player.posY >= 245.0f) {
					player.posY = 245.0f;
					player.isJumping = false;
					player.v.y = 0.0f;
				}
			}
			else {
				player.posY += 5.0f;
				if (player.posY >= 245.0f) {
					player.posY = 245.0f;
				}
			}

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				if (!bullet.isBulletFlag) {
					bullet.posX = player.posX;
					bullet.posY = player.posY;
					bullet.isBulletFlag = true;
					bullet.speed = player.isFacingRight ? 5.0f : -5.0f;
				}
			}

			if (bullet.isBulletFlag) {
				bullet.posX += bullet.speed;

				if (bullet.posX >= 640 || bullet.posX <= 0) {
					bullet.isBulletFlag = false;
				}

				// 弾と敵の当たり判定
				if (CheckCollision(
					bullet.posX, bullet.posY, bullet.radius, bullet.radius,
					enemy.posX, enemy.posY, enemy.w, enemy.h)) {

					enemy.lives--;  // 敵のライフを減少

					if (enemy.lives <= 0 && enemy.isAlive) {
						enemy.isAlive = false;
						CreateParticles(enemy, particles); // 爆発パーティクルの生成
					}
					// 敵がダメージを受けたときのエフェクト
					enemy.isDamaged = true;
					enemy.damageTimer = 0.5f;  // ダメージエフェクトの表示時間

					bullet.isBulletFlag = false;
				}
			}

			// ダメージエフェクトのタイマーを更新
			if (enemy.isDamaged) {
				enemy.damageTimer -= 0.016f;  // 60FPSと仮定
				if (enemy.damageTimer <= 0.0f) {
					enemy.isDamaged = false;  // ダメージエフェクトが終了したらフラグをオフにする
				}
			}

			// パーティクルの更新
			for (auto& particle : particles) {
				particle.x += particle.vx;
				particle.y += particle.vy;
				particle.lifetime -= 0.05f;

				if (particle.lifetime <= 0.0f) {
					particle.vx = 0.0f;
					particle.vy = 0.0f;
					isParticleEffectActive = false;
				}
			}

			// パーティクルの削除
			particles.erase(
				std::remove_if(
					particles.begin(),
					particles.end(),
					[](const Particle& p) { return p.lifetime <= 0.0f; }), particles.end());


			if (particles.empty() && !isParticleEffectActive) {
				sceneNo = CLEAR;
			}

			 case CLEAR:
				 // CLEARシーンの更新処理
				 if (keys[DIK_R] && !preKeys[DIK_R]) {
					 // Rキーが押されたらタイトル画面に戻る
					 sceneNo = MODE;
					 // ゲーム画面の初期化
					 player = { 100.0f, 245.0f, 3.0f, 15.0f, 15.0f, 3.0f, false, false, true };
					 enemy = { 400.0f, 240.0f, 3.0f, 20.0f, 20.0f, 3, 3, true, false, 0.0f };
					 bullet = { 0.0f, 0.0f, 5.0f, 3.0f, false };
					 particles.clear();
					 isParticleEffectActive = true;

				 }
			break;
		}



		// 描画処理
		switch (sceneNo) {
		case TITLE:
			// 白の背景
			Novice::DrawBox(
				static_cast<int>(posX1),
				static_cast<int>(scenePosY),
				640, 360,
				0.0f, 0x0a0a66ff, kFillModeSolid);

			// 文字の描画 (ブレンディング)
			Novice::DrawSprite(
				static_cast<int>(scenePosX + 200),
				static_cast<int>(scenePosY + 240),
				SPACE_GH,
				1.0f, 1.0f, 0.0f, color | (alpha << 24));
			Novice::DrawSprite(
				static_cast<int>(scenePosX + 290),
				static_cast<int>(scenePosY + 240),
				TO_GH,
				1.0f, 1.0f, 0.0f, color | (alpha << 24));
			Novice::DrawSprite(
				static_cast<int>(scenePosX + 390),
				static_cast<int>(scenePosY + 240),
				START_GH,
				1.0f, 1.0f, 0.0f, color | (alpha << 24));

			Novice::ScreenPrintf(12, 24, "SPACE:MODE");

			break;

		case MODE:
			// 黒の背景
			Novice::DrawBox(
				static_cast<int>(posX1),
				static_cast<int>(scenePosY),
				640, 360,
				0.0f, 0x000000ff, kFillModeSolid);

			Novice::ScreenPrintf(12, 24, "1:GAME");


			break;

		case GAME:

			// 黒の背景
			Novice::DrawBox(
				0,
				0,
				640, 360,
				0.0f, 0xBDBDBDff, kFillModeSolid);

			// 描画処理
			DrawSnow();

			Novice::DrawBox(
				0, 260,
				640, 360,
				0.0f, 0xA9E2F3ff, kFillModeSolid);

			//自機の描画
			Novice::DrawBox(
				static_cast<int>(player.posX),
				static_cast<int>(player.posY),
				static_cast<int>(player.w),
				static_cast<int>(player.h),
				0.0f,
				0xffffffff,
				kFillModeSolid);

			// パーティクルの描画
			for (const auto& particle : particles) {
				Novice::DrawEllipse(
					static_cast<int>(particle.x),
					static_cast<int>(particle.y),
					3, 3, 0.0f, RED, kFillModeSolid
				);
			}

			// 敵の描画
			if (enemy.isAlive) {
				if (enemy.isDamaged) {
					// ダメージを受けている場合は点滅させる
					if (static_cast<int>(enemy.damageTimer * 10) % 2 == 0) {
						Novice::DrawBox(
							static_cast<int>(enemy.posX),
							static_cast<int>(enemy.posY),
							static_cast<int>(enemy.w),
							static_cast<int>(enemy.h),
							0.0f,
							0xff0000ff,
							kFillModeSolid);
					}
				}
				else {
					Novice::DrawBox(
						static_cast<int>(enemy.posX),
						static_cast<int>(enemy.posY),
						static_cast<int>(enemy.w),
						static_cast<int>(enemy.h),
						0.0f,
						enemy.isAlive ? 0x000000ff : 0xff0000ff,
						kFillModeSolid);
				}
			}

			//弾の描画
			if (bullet.isBulletFlag) {
				Novice::DrawEllipse(
					static_cast<int>(bullet.posX),
					static_cast<int>(bullet.posY),
					static_cast<int>(bullet.radius),
					static_cast<int>(bullet.radius),
					0.0f,
					RED,
					kFillModeSolid);
			}


			//座標の表示
			Novice::ScreenPrintf(12, 24, "playerPosX=%f", player.posX);
			Novice::ScreenPrintf(12, 48, "playerPosY=%f", player.posY);
			Novice::ScreenPrintf(12, 72, "x=%f", bullet.posX);
			Novice::ScreenPrintf(12, 96, "Enemy Lives: %d", enemy.lives);

			Novice::ScreenPrintf(450, 24, "RETURN:TITLE");
			Novice::ScreenPrintf(450, 48, "playerMove: A & D");
			Novice::ScreenPrintf(450, 72, "playerJump: W");
			Novice::ScreenPrintf(450, 96, "playerBullet: SPACE");

			break;

		case CLEAR:

			// CLEARシーンの描画処理
			Novice::DrawBox(
				0, 0,
				640, 360,
				0.0f, 0x1C1C1Cff, kFillModeSolid);

			Novice::ScreenPrintf(12, 24, "R:MODE");
			Novice::ScreenPrintf(250, 180, "cleared the game!");

			break;
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}