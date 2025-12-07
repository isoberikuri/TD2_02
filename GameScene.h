#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Bullet.h"

class GameScene
{
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

	//=============//
	//   弾選択用  //
	//=============//
	
	void SetCardTexture(Sprite* sprite, int cardNo);
	void ChangeBulletModel(int type);
	std::string GetCardPath(int type);
	int bulletSelectPoint_ = 0;
	int bulletSelectTime_ = 300; //1800 = 30秒
	//セレクトカード選択
	int Select1Point_ = 0;
	int Select2Point_ = 0;
	int SelectSet_ = 1;
	// セレクトカード画像ハンドル
	uint32_t bulletSelect1Handle_ = 0;
	uint32_t bulletSelect2Handle_ = 0;
	// セレクトカードスプライト
	Sprite* bulletSelect1Sprite_ = nullptr;
	Sprite* bulletSelect2Sprite_ = nullptr;
	// セレクトカードの現在の座標（X位置は固定、Yが上下移動）
	float bulletSelect1PosX_ = 197.0f;
	float bulletSelect1PosY_ = 40.0f;
	float bulletSelect2PosX_ = 680.0f;
	float bulletSelect2PosY_ = 40.0f;

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