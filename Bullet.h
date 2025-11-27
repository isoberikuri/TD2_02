#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "Player.h"

class Bullet
{
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// ワールド座標の取得
	KamataEngine::Vector3 GetWorldPosition();

	// ワールド取得
	const KamataEngine::WorldTransform& GetWorldTransform() const { return bulletWorldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return bulletVelocity_; }
	
	//弾をプレイヤーの位置に持っていく用
	void SetPlayer(Player* player) { player_ = player; }

	//==========//
	//  弾種類  //
	//==========//
	//普通
	int bullet1Normal_ = false;
	//反射
	int bullet2Reflect_ = false;
	//操作
	int bullet3Operation_ = false;
	//連射
	int bullet4Rapid_ = false;
	struct BulletData
	{
		bool active = false;
		KamataEngine::Vector3 position;
		KamataEngine::Vector2 velocity;
	};
	static const int MAX_BULLETS = 20; // 連射数
	BulletData bullets_[MAX_BULLETS];
	//3方向
	int bullet5Trident_ = false;

	//==========//
	//  全弾用  //
	//==========//
	int bulletPoint_ = true;
	float bulletSpeed_ = 0.0f;
	void SetModel(KamataEngine::Model* model);


private:
	// ===== 基本データ =====
	KamataEngine::WorldTransform bulletWorldTransform_;
	KamataEngine::Camera* bulletCamera_ = nullptr;
	KamataEngine::Model* bulletModel_ = nullptr;

	// ===== 移動 =====
	KamataEngine::Vector3 bulletVelocity_ = {};

	// 弾の加速度（速度を増やすときの増加量）
	static inline const float kBulletAcceleration = 0.1f;
	// 弾の減速度（速度を減らすときの減少量、慣性の減衰などに利用）
	static inline const float kBulletAttenuation = 0.1f;
	// 弾の速度の最大値（速度の上限を制限して速くなりすぎないようにする）
	static inline const float kBulletLimitSpeed = 0.2f;

	// 弾をプレイヤーの位置に持っていく用
	Player* player_ = nullptr;

};