#include "GameScene.h"
#include "Player.h"
#include "Bullet.h"
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;

void GameScene::Initialize()
{

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

	// セレクトカード画像読み込み
	bulletSelect1Handle_ = TextureManager::Load("BulletCard/NormalBulletCard.png");
	bulletSelect1Sprite_ = Sprite::Create(bulletSelect1Handle_, {bulletSelect1PosX_, bulletSelect1PosY_});
	bulletSelect2Handle_ = TextureManager::Load("BulletCard/NormalBulletCard.png");
	bulletSelect2Sprite_ = Sprite::Create(bulletSelect2Handle_, {bulletSelect2PosX_, bulletSelect2PosY_});
	std::srand((unsigned int)std::time(nullptr));

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

//=========================//
//     カード画像変更      //
//=========================//

void GameScene::SetCardTexture(Sprite* sprite, int cardNo)
{ 
	sprite->SetTextureHandle(TextureManager::Load(GetCardPath(cardNo)));
}

//=========================//
//       モデル切替        //
//=========================//

void GameScene::ChangeBulletModel(int type)
{
	delete modelBullet_;
	switch (type) {
	case 1:
		modelBullet_ = Model::CreateFromOBJ("bullet1", true);
		break;
	case 2:
		modelBullet_ = Model::CreateFromOBJ("bullet2", true);
		break;
	case 3:
		modelBullet_ = Model::CreateFromOBJ("bullet3", true);
		break;
	case 4:
		modelBullet_ = Model::CreateFromOBJ("bullet4", true);
		break;
	case 5:
		modelBullet_ = Model::CreateFromOBJ("bullet5", true);
		break;
	}
	bullet_->SetModel(modelBullet_);
}
std::string GameScene::GetCardPath(int type)
{
	switch (type)
	{
	case 1:
		return "BulletCard/NormalBulletCard.png";
	case 2:
		return "BulletCard/ReflectBulletCard.png";
	case 3:
		return "BulletCard/OperationBulletCard.png";
	case 4:
		return "BulletCard/SpeedChangeBulletCard.png";
	case 5:
		return "BulletCard/TridentBulletCard.png";
	}
	return "BulletCard/NormalBulletCard.png";
}
void GameScene::Update() {


	//=====================//
	//    弾・モデル切替   //
	//=====================//
	int currentType = bullet_->GetCurrentBulletType();

	if (currentType != prevBulletType_)
	{
		ChangeBulletModel(currentType);
	}
	prevBulletType_ = currentType;

	//=======================//
	//   カード選択フェーズ  //
	//=======================//
	
	if (bulletSelectPoint_ == 1)
	{
		if (Input::GetInstance()->TriggerKey(DIK_A))
			SelectSet_ = 1;

		if (Input::GetInstance()->TriggerKey(DIK_D))
			SelectSet_ = 2;

		// ★ どちらを選択しているかによって拡大率変更
		if (SelectSet_ == 1)
		{
			bulletSelect1Sprite_->SetSize({336 * 1.2f, 528 * 1.2f});
			bulletSelect2Sprite_->SetSize({336, 528});
		}
		else
		{
			bulletSelect1Sprite_->SetSize({336, 528});
			bulletSelect2Sprite_->SetSize({336 * 1.2f, 528 * 1.2f});
		}

		// SPACE 決定
		if (Input::GetInstance()->TriggerKey(DIK_RETURN))
		{
			int chosenCard = (SelectSet_ == 1) ? Select1Point_ : Select2Point_;

			bullet_->SetBulletType(chosenCard);
			ChangeBulletModel(chosenCard);

			bulletSelectPoint_ = 0;
			bulletSelectTime_ = 300;
		}
	}
	
	//===========================//
	//      プレイフェーズ       //
	//===========================//
	if (bulletSelectPoint_ == 0)
	{
		bulletSelectTime_--;

		if (bulletSelectTime_ <= 0)
		{
			bulletSelectPoint_ = 1;
			bullet_->ResetBulletState();

			// ランダムカード生成
			Select1Point_ = (std::rand() % 5) + 1;
			Select2Point_ = (std::rand() % 5) + 1;

			// カード画像切替
			SetCardTexture(bulletSelect1Sprite_, Select1Point_);
			SetCardTexture(bulletSelect2Sprite_, Select2Point_);

			SelectSet_ = 1;
		}
		//==========//
		//	その他  //
		//==========//
		// プレイヤー
		if (player_)
		{
			player_->Update();
		}
		// 弾
		if (bullet_)
		{
			bullet_->Update();
		}
		camera_.TransferMatrix();
	}
}

void GameScene::Draw()
{

	//=========================//
	//    2Dスプライト描画     //
	//=========================//
	if (bulletSelectPoint_ == 1) // ←追加
	{
		Sprite::PreDraw();

		// セレクトカードを描画
		bulletSelect1Sprite_->Draw();
		bulletSelect2Sprite_->Draw();

		// スプライト描画後処理
		Sprite::PostDraw();
	}

	//=========================//
	//    3Dスプライト描画     //
	//=========================//
	
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