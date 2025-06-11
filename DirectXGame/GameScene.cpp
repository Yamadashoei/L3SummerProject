#include "GameScene.h"
#include "Player.h"

#include <cassert>

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	// システム系の初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// カメラ初期化
	camera.Initialize();
	camera.translation_ = {0.0f, 0.0f, -50.0f}; // TPS用に少し後方に置く
	camera.rotation_ = {0.0f, 0.0f, 0.0f};      // 正面向き
	camera.UpdateMatrix();

	// プレイヤーモデル読み込み
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("cube"); // 仮cubeモデル

	// プレイヤー生成・初期化
	player_ = new Player();
	player_->Initialize(modelPlayer_);

	// デバッグカメラ生成
	debugCamera_ = new KamataEngine::DebugCamera(KamataEngine::WinApp::kWindowWidth, KamataEngine::WinApp::kWindowHeight);

	// 軸表示
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&camera);
}

void GameScene::Update() {
	// プレイヤー更新
	player_->Update();

#ifdef _DEBUG
	// デバッグカメラ切り替え
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera.matView = debugCamera_->GetCamera().matView;
		camera.matProjection = debugCamera_->GetCamera().matProjection;
		camera.TransferMatrix();
	} else {
		camera.UpdateMatrix();
	}
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	Model::PreDraw(commandList);
	player_->Draw(camera);
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
#pragma endregion
}
