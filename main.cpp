#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// DirectX12
DirectXCommon* dxCommon = DirectXCommon::GetInstance();

// ゲームシーン
GameScene* gameScene = nullptr;

// Windowsアプリのエントリーポイント
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジン初期化
	KamataEngine::Initialize(L"2262_エアフォース");

	// GameScene を生成
	gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {

		// エンジンの更新（閉じるボタンで終了）
		if (KamataEngine::Update()) {
			break;
		}

		// ゲーム更新
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// ゲーム描画
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 解放
	delete gameScene;

	// エンジン終了
	KamataEngine::Finalize();

	return 0;
}
