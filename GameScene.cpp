#include "GameScene.h"
#include "Player.h"
#include "Bullet.h"

using namespace KamataEngine;

void GameScene::Initialize() {

     // ===== カメラ初期化（必ず最初）=====
	camera_.Initialize();

	// ===== ワールド初期化 =====
	worldTransform_.Initialize();

	// プレイヤー用モデル読み込み
	modelPlayer_ = Model::CreateFromOBJ("player", true);

	// 弾オブジェクトを先に作成する
	bullet_ = new Bullet();
	modelBullet_ = Model::CreateFromOBJ("bullet1", true);

	// ===== プレイヤー生成 =====
	player_ = new Player();
	Vector3 playerPos = {0, 0, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPos);

	// ===== 弾生成 =====
	Vector3 bulletPos = {0, 0, 0};
	bullet_->Initialize(modelBullet_, &camera_, bulletPos);

	// ★ ここで必ず SetPlayer する！
	bullet_->SetPlayer(player_);
}

GameScene::~GameScene()
{
    //自機
    delete player_;
    delete modelPlayer_;
    //弾
	delete bullet_;
	delete modelBullet_;
}

void GameScene::Update() {


	//=====================//
	//    弾の描画変更用   //
	//=====================//
	int currentType = 0;

	if (bullet_->bullet1Normal_ == 1)
		currentType = 1;
	if (bullet_->bullet2Reflect_ == 1)
		currentType = 2;
	if (bullet_->bullet3Operation_ == 1)
		currentType = 3;
	if (bullet_->bullet4Rapid_ == 1)
		currentType = 4;
	if (bullet_->bullet5Trident_ == 1)
		currentType = 5;

	// 種類が変わった時だけ実行する
	if (currentType != prevBulletType_) {

		delete modelBullet_;

		if (currentType == 1)
			modelBullet_ = Model::CreateFromOBJ("bullet1", true);
		if (currentType == 2)
			modelBullet_ = Model::CreateFromOBJ("bullet2", true);
		if (currentType == 3)
			modelBullet_ = Model::CreateFromOBJ("bullet3", true);
		if (currentType == 4)
			modelBullet_ = Model::CreateFromOBJ("bullet4", true);
		if (currentType == 5)
			modelBullet_ = Model::CreateFromOBJ("bullet5", true);

		bullet_->SetModel(modelBullet_);
	}
	prevBulletType_ = currentType;

	//==========//
	//	その他  //
	//==========//
	// プレイヤー
    if (player_) {
        player_->Update();
    }
	// 弾
	if (bullet_) {
		bullet_->Update();
	}


    // カメラは固定（プレイヤー追従しない）
    camera_.TransferMatrix();
}

void GameScene::Draw()
{
	Model::PreDraw();

    //自機
	if (player_) {
		player_->Draw();
	}
	// 弾
	if (bullet_) {
		bullet_->Draw();
	}

	Model::PostDraw();
}