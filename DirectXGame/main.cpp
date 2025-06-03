#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE3C_27_ヤマダ_ショウエイ");
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	
	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		

		// 描画開始 ↓ここから
		dxCommon->PreDraw();

		
		// 描画終了
		dxCommon->PostDraw();
	}

	
	
	// 解放処理
	KamataEngine::Finalize();

	return 0;
}
