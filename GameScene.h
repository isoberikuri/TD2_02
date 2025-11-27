#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Bullet.h"

class GameScene {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	// デストラクタ
	~GameScene();

	//弾の描画変更用
	int prevBulletType_ = 0;

private:
	// ===== プレイヤー =====
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// ===== 弾 =====
	Bullet* bullet_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;

	// ===== カメラ =====
	KamataEngine::Camera camera_;

	// ===== ワールドトランスフォーム（必要最低限） =====
	KamataEngine::WorldTransform worldTransform_;
};