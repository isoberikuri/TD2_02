#define NOMINMAX
#include "Player.h"
#include <algorithm>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, Vector3& position)
{
	assert(model);

	playerModel_ = model;
	playerCamera_ = camera;

	// ★ translation を先に入れる
	playerWorldTransform_.translation_ = position;

	// ★ Initialize を後で呼ぶ（これが正しい）
	playerWorldTransform_.Initialize();
}

void Player::Update()
{
	// 入力による移動更新
	InputMove();

	// 速度を位置に反映
	playerWorldTransform_.translation_.x += playerVelocity_.x;
	playerWorldTransform_.translation_.y += playerVelocity_.y;

	// ★ 毎フレーム行列を作成
	playerWorldTransform_.matWorld_ = MakeAffineMatrix(playerWorldTransform_.scale_, playerWorldTransform_.rotation_, playerWorldTransform_.translation_);

	// ★ GPUに転送
	playerWorldTransform_.TransferMatrix();
}

void Player::Draw()
{ 
	playerModel_->Draw(playerWorldTransform_, *playerCamera_);
}

// 移動入力処理（シンプル）
void Player::InputMove()
{
	float moveSpeed = 0.8f;

	Vector2 move = {0.0f, 0.0f};

	if (Input::GetInstance()->PushKey(DIK_D))
	{
		move.x += moveSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		move.x -= moveSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		move.y += moveSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		move.y -= moveSpeed;
	}

	playerVelocity_.x = move.x;
	playerVelocity_.y = move.y;
}