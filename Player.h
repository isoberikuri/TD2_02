#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Player {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 入力による移動処理
	void InputMove();

	// 描画
	void Draw();

	// ===== ワールド座標の取得 =====
	inline KamataEngine::Vector3 GetWorldPosition() const { return {playerWorldTransform_.matWorld_.m[3][0], playerWorldTransform_.matWorld_.m[3][1], playerWorldTransform_.matWorld_.m[3][2]}; }

	// ワールド情報取得
	const KamataEngine::WorldTransform& GetWorldTransform() const { return playerWorldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return playerVelocity_; }

private:
	// ===== 基本データ =====
	KamataEngine::WorldTransform playerWorldTransform_;
	KamataEngine::Camera* playerCamera_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;

	// ===== 移動 =====
	KamataEngine::Vector3 playerVelocity_ = {};

	static inline const float playerKAccleration = 0.1f;
	static inline const float playerKAttenuation = 0.1f;
	static inline const float playerKLimitRunSpeed = 0.5f;
};