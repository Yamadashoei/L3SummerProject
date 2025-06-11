#pragma once
#include "KamataEngine.h"

#include "Player.h"
#include "PlayerBullet.h"

using namespace KamataEngine;

class GameScene {

public:
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

private:
	// カメラ
	KamataEngine::Camera camera;

	// DirectX / 入力 / オーディオ
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// モデル
	KamataEngine::Model* modelPlayer_ = nullptr;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	// プレイヤー
	Player* player_ = nullptr;
};
