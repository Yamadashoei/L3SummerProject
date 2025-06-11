#include "KamataEngine.h"
#include <Windows.h>

#include "MainSceneManager.h"

using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE3C_27_ヤマダ_ショウエイ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ★ MainSceneManagerの初期化
	MainSceneManager* mainSceneManager = new MainSceneManager();
	mainSceneManager->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// ★ MainSceneManagerのアップデート
		mainSceneManager->Update();

		// 描画開始
		dxCommon->PreDraw();

		// ★ MainSceneManagerの描画
		mainSceneManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// メインループ終了後
	mainSceneManager->Finalize();
	// ★ MainSceneManagerの解放
	delete mainSceneManager;
	mainSceneManager = nullptr;

	// 解放処理
	KamataEngine::Finalize();

	return 0;
}
