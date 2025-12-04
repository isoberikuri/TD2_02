#include "Bullet.h"
#define NOMINMAX
#include <algorithm>
#include <numbers>
#include "Player.h"

using namespace KamataEngine;

void Bullet::Initialize(Model* model, Camera* camera, Vector3& position)
{
	assert(model);
	bulletModel_ = model;
	bulletWorldTransform_.translation_ = position;
	bulletCamera_ = camera;
	bulletWorldTransform_.Initialize();

	// トライデント用
	for (int i = 0; i < 3; i++)
	{
		bullet5WorldTransform_[i].translation_ = position;
		bullet5WorldTransform_[i].translation_.x += i;
		bullet5WorldTransform_[i].Initialize();
	}

	// SetPlayerは外部で呼ぶのでここでは呼ばない
}

Vector3 Bullet::GetWorldPosition()
{

	Vector3 bulletWorldPos;
	// ワールド行列の平行移動成分（弾の現在位置）を取得
	bulletWorldPos.x = bulletWorldTransform_.matWorld_.m[3][0];
	bulletWorldPos.y = bulletWorldTransform_.matWorld_.m[3][1];
	bulletWorldPos.z = bulletWorldTransform_.matWorld_.m[3][2];
	return bulletWorldPos;

}

// トライデント用
KamataEngine::Vector3 Bullet::bullet5GetWorldPosition(int i)
{
	Vector3 bullet5WorldPos;
	// ワールド行列の平行移動成分（弾の現在位置）を取得
	bullet5WorldPos.x = bullet5WorldTransform_[i].matWorld_.m[3][0];
	bullet5WorldPos.y = bullet5WorldTransform_[i].matWorld_.m[3][1];
	bullet5WorldPos.z = bullet5WorldTransform_[i].matWorld_.m[3][2];
	return bullet5WorldPos;
}

void Bullet::Update()
{
	//==========//
	//	その他  //
	//==========//
	
	// --- 弾3がアクティブで「発射中」のときはここをスキップ ---
	
	// ワールド行列を作成（スケール、回転、移動を合成）
	bulletWorldTransform_.matWorld_ = MakeAffineMatrix(bulletWorldTransform_.scale_, bulletWorldTransform_.rotation_, bulletWorldTransform_.translation_);
	// 行列をGPUなどに転送して描画準備
	bulletWorldTransform_.TransferMatrix();
	for (int i = 0; i < 3; i++)
	{
		// ワールド行列を作成（スケール、回転、移動を合成）
		bullet5WorldTransform_[i].matWorld_ = MakeAffineMatrix(bullet5WorldTransform_[i].scale_, bullet5WorldTransform_[i].rotation_, bullet5WorldTransform_[i].translation_);
		// 行列をGPUなどに転送して描画準備
		bullet5WorldTransform_[i].TransferMatrix();
	}

	//==========//
	//    弾1   //
	//==========//
	
	if (Input::GetInstance()->PushKey(DIK_1))
	{
		bullet1Normal_ = 1;
		bullet2Reflect_ = 0;
		bullet3Operation_ = 0;
		bullet4SpeedChange_ = 0;
		bullet5Trident_ = 0;

		// 弾の速度
		bulletSpeed_ = 0.8f;
	}
	if (bullet1Normal_ == 1)
	{
		if (bulletPoint_ == 1)
		{
			// SPACE を押したら発射
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletPoint_ = 0; // 発射中;

				// 自機に移動
				bulletWorldTransform_.translation_ = player_->GetWorldPosition();

				// 上方向へ発射
				bulletVelocity_.x = 0.0f;
				bulletVelocity_.y = bulletSpeed_;
			}
		}
		else
		{

			bulletVelocity_.y = bulletSpeed_;

			// ★ 弾3の独自移動処理（ここが弾の移動）
			bulletWorldTransform_.translation_.x += bulletVelocity_.x;
			bulletWorldTransform_.translation_.y += bulletVelocity_.y;
		}

		// 画面外に出たらリセット
		const float SCREEN_TOP = 20.0f;
		if (bulletWorldTransform_.translation_.y > SCREEN_TOP)
		{
			bulletVelocity_ = {0.0f, 0.0f};
			bulletPoint_ = 1;
			bulletWorldTransform_.translation_.y = -1000.0f;
		}
	}
	
	//==========//
	//    弾2   //
	//==========//
	
	if (Input::GetInstance()->PushKey(DIK_2))
	{
		bullet1Normal_ = 0;
		bullet2Reflect_ = 1;
		bullet3Operation_ = 0;
		bullet4SpeedChange_ = 0;
		bullet5Trident_ = 0;
		
		// 弾の速度
		bulletSpeed_ = 0.3f;
	}
	if (bullet2Reflect_ == 1)
	{
		if (bulletPoint_ == 1)
		{
			// SPACE を押したら発射
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletPoint_ = 0; // 発射中;

				// 自機に移動
				bulletWorldTransform_.translation_ = player_->GetWorldPosition();

				// 斜め上方向に発射（例）
				bulletVelocity_.x = bulletSpeed_;
				bulletVelocity_.y = bulletSpeed_;
			}
		}
		else
		{
			// 弾の移動
			bulletWorldTransform_.translation_.x += bulletVelocity_.x;
			bulletWorldTransform_.translation_.y += bulletVelocity_.y;

			// 画面端の範囲（適宜調整）
			const float SCREEN_LEFT = -36.0f;
			const float SCREEN_RIGHT = 36.0f;
			const float SCREEN_BOTTOM = -20.0f;
			const float SCREEN_TOP = 20.0f;

			// 横方向の端判定と反転
			if (bulletWorldTransform_.translation_.x < SCREEN_LEFT)
			{
				bulletVelocity_.x = -bulletVelocity_.x;
				bulletWorldTransform_.translation_.x = SCREEN_LEFT; // はみ出し防止
			}
			if (bulletWorldTransform_.translation_.x > SCREEN_RIGHT)
			{
				bulletVelocity_.x = -bulletVelocity_.x;
				bulletWorldTransform_.translation_.x = SCREEN_RIGHT; // はみ出し防止
			}

			// 縦方向の端判定と反転
			if (bulletWorldTransform_.translation_.y < SCREEN_BOTTOM)
			{
				bulletVelocity_.y = -bulletVelocity_.y;
				bulletWorldTransform_.translation_.y = SCREEN_BOTTOM; // はみ出し防止
			}
			if (bulletWorldTransform_.translation_.y > SCREEN_TOP)
			{
				bulletVelocity_.y = -bulletVelocity_.y;
				bulletWorldTransform_.translation_.y = SCREEN_TOP; // はみ出し防止
			}

			// SPACE を押したら速度ベクトルを反転
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletVelocity_.x = -bulletVelocity_.x;
				//bulletVelocity_.y = -bulletVelocity_.y;
			}
		}
	}
	
	//==========//
	//    弾3   //
	//==========//
	
	if (Input::GetInstance()->PushKey(DIK_3))
	{
		bullet1Normal_ = 0;
		bullet2Reflect_ = 0;
		bullet3Operation_ = 1;
		bullet4SpeedChange_ = 0;
		bullet5Trident_ = 0;

		//弾の速度
		bulletSpeed_ = 0.5f;
	}
	if (bullet3Operation_ == 1)
	{
		if (bulletPoint_ == 1)
		{
			// SPACE を押したら発射
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletPoint_ = 0; // 発射中;

				//自機に移動
				bulletWorldTransform_.translation_ = player_->GetWorldPosition();

				// 上方向へ発射
				bulletVelocity_.x = 0.0f;
				bulletVelocity_.y = bulletSpeed_;
			}
		}
		else
		{
			// --- 発射後の軌道調整 ---
			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				bulletVelocity_.x = bulletSpeed_;
			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				bulletVelocity_.x = -bulletSpeed_;
			}
			else
			{
				bulletVelocity_.x = 0.0f;
			}

			bulletVelocity_.y = bulletSpeed_;

			// ★ 弾3の独自移動処理（ここが弾の移動）
			bulletWorldTransform_.translation_.x += bulletVelocity_.x;
			bulletWorldTransform_.translation_.y += bulletVelocity_.y;
		}

		// 画面外に出たらリセット
		const float SCREEN_TOP = 20.0f;
		if (bulletWorldTransform_.translation_.y > SCREEN_TOP)
		{
			bulletVelocity_ = {0.0f, 0.0f};
			bulletPoint_ = 1;
			bulletWorldTransform_.translation_.y = -1000.0f;
		}
	}

	//==========//
	//    弾4   //
	//==========//
	
	if (Input::GetInstance()->PushKey(DIK_4))
	{
		bullet1Normal_ = 0;
		bullet2Reflect_ = 0;
		bullet3Operation_ = 0;
		bullet4SpeedChange_ = 1;
		bullet5Trident_ = 0;

		// 弾の速度
		bulletSpeed_ = 0.6f;
	}

	if (bullet4SpeedChange_ == 1)
	{
		if (bulletPoint_ == 1)
		{
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletPoint_ = 0;

				bulletWorldTransform_.translation_ = player_->GetWorldPosition();

				// 発射時の初速
				currentSpeed_ = bulletSpeed_;
			}
		}
		else
		{
			// --- 速度を加減速する部分 ---
			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				currentSpeed_ += bullet4Acceleration_; // 加速
			}
			else if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				currentSpeed_ -= bullet4Acceleration_; // 減速
			}

			// 速度制限
			currentSpeed_ = std::clamp(currentSpeed_, minSpeed_, maxSpeed_);

			// 弾速度に反映
			bulletVelocity_.x = 0.0f;
			bulletVelocity_.y = currentSpeed_;

			// 弾を移動
			bulletWorldTransform_.translation_.y += bulletVelocity_.y;
		}

		// 画面外に出たらリセット
		const float SCREEN_TOP = 20.0f;
		const float SCREEN_BOTTOM = -20.0f;
		if (bulletWorldTransform_.translation_.y > SCREEN_TOP)
		{
			bulletVelocity_ = {0.0f, 0.0f};
			bulletPoint_ = 1;
			currentSpeed_ = 0.0f; // 速度リセット
			bulletWorldTransform_.translation_.y = -1000.0f;
		}
		if (bulletWorldTransform_.translation_.y < SCREEN_BOTTOM)
		{
			bulletVelocity_ = {0.0f, 0.0f};
			bulletPoint_ = 1;
			currentSpeed_ = 0.0f; // 速度リセット
			bulletWorldTransform_.translation_.y = -1000.0f;
		}
	}

	//==========//
	//    弾5   //
	//==========//
	
	if (Input::GetInstance()->PushKey(DIK_5))
	{
		bullet1Normal_ = 0;
		bullet2Reflect_ = 0;
		bullet3Operation_ = 0;
		bullet4SpeedChange_ = 0;
		bullet5Trident_ = 1;

		// 弾の速度
		bulletSpeed_ = 0.6f;
	}
	if (bullet5Trident_ == 1)
	{
		if (bulletPoint_ == 1)
		{
			if (Input::GetInstance()->TriggerKey(DIK_SPACE))
			{
				bulletPoint_ = 0;

				Vector3 base = player_->GetWorldPosition();

				float offsetX[3] = {0.0f, -0.1f, 0.1f};

				for (int i = 0; i < 3; i++)
				{
					bullet5WorldTransform_[i].translation_ = base;

					bullet5Velocity_[i].x = offsetX[i];
					bullet5Velocity_[i].y = bulletSpeed_;
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				bullet5WorldTransform_[i].translation_.x += bullet5Velocity_[i].x;
				bullet5WorldTransform_[i].translation_.y += bullet5Velocity_[i].y;
			}
		}

		// ---- 正しい画面外処理 ----
		const float SCREEN_TOP = 20.0f;

		for (int i = 0; i < 3; i++)
		{
			if (bullet5WorldTransform_[i].translation_.y > SCREEN_TOP)
			{
				// 3つまとめてリセット
				for (int j = 0; j < 3; j++)
				{
					bullet5WorldTransform_[j].translation_.y = -1000.0f;
					bullet5Velocity_[j] = {0.0f, 0.0f};
				}
				bulletPoint_ = 1;
			}
		}
	}

}

void Bullet::SetModel(Model* model) { bulletModel_ = model; }

void Bullet::Draw()
{
	if (bullet1Normal_ == 1)
	{
		if (bulletPoint_ == 0)
		{
			// モデルを現在の変換行列とカメラ情報を使って描画
			bulletModel_->Draw(bulletWorldTransform_, *bulletCamera_);
		}
	}

	if (bullet2Reflect_ == 1)
	{
		if (bulletPoint_ == 0)
		{
			// モデルを現在の変換行列とカメラ情報を使って描画
			bulletModel_->Draw(bulletWorldTransform_, *bulletCamera_);
		}
	}

	if (bullet3Operation_ == 1)
	{
		if (bulletPoint_ == 0)
		{
			// モデルを現在の変換行列とカメラ情報を使って描画
			bulletModel_->Draw(bulletWorldTransform_, *bulletCamera_);
		}
	}

	if (bullet4SpeedChange_ == 1)
	{
		if (bulletPoint_ == 0)
		{
			// モデルを現在の変換行列とカメラ情報を使って描画
			bulletModel_->Draw(bulletWorldTransform_, *bulletCamera_);
		}
	}

	if (bullet5Trident_ == 1)
	{
		if (bulletPoint_ == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				// モデルを現在の変換行列とカメラ情報を使って描画
				bulletModel_->Draw(bullet5WorldTransform_[i], *bulletCamera_);
			}
		}
	}

}