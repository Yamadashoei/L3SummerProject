#include "GameScene.h"
#include "MidEnemy.h"
#include "Player.h"
#include "SmallEnemy.h"
// #include "BigEnemy.h" // ← BigEnemy作成時に追加予定

#include <cassert>

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelMidEnemy_;
	delete modelBigEnemy_;

	delete player_;
	delete debugCamera_;

	for (BaseEnemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	camera.Initialize();
	camera.translation_ = {0.0f, 0.0f, -50.0f};
	camera.rotation_ = {0.0f, 0.0f, 0.0f};
	camera.UpdateMatrix();

	modelPlayer_ = Model::CreateFromOBJ("cube");
	modelEnemy_ = Model::CreateFromOBJ("cube");    // 仮モデル
	modelMidEnemy_ = Model::CreateFromOBJ("cube"); // 仮モデル
	modelBigEnemy_ = Model::CreateFromOBJ("cube"); // 仮モデル

	player_ = new Player();
	player_->Initialize(modelPlayer_);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);

	// ★ SmallEnemy生成
	SmallEnemy* shootEnemy = new SmallEnemy();
	shootEnemy->Initialize(modelEnemy_, {0.0f, 5.0f, 10.0f}, SmallEnemy::AttackType::Shoot);
	enemies_.push_back(shootEnemy);

	SmallEnemy* ramEnemy = new SmallEnemy();
	ramEnemy->Initialize(modelEnemy_, {-5.0f, 3.0f, 20.0f}, SmallEnemy::AttackType::Ram);
	enemies_.push_back(ramEnemy);

	// ★ MidEnemy生成
	MidEnemy* midEnemy = new MidEnemy();
	midEnemy->Initialize(modelMidEnemy_, {5.0f, 0.0f, 15.0f});
	enemies_.push_back(midEnemy);

	// BigEnemyは後で同様に追加予定

	delete stage_;
	stage_ = new Stage1();
	stage_->Initialize(modelEnemy_, modelMidEnemy_, player_);

}

void GameScene::Update() {
	player_->Update();

#ifdef _DEBUG
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

	// ★ Stage更新
	if (stage_) {
		stage_->Update();
	}

	// Player vs Enemy
	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		if (player_->GetCollision().CheckCollision(enemy->GetCollision())) {
			player_->TakeDamage(20);
			player_->SetHit();
			OutputDebugStringA("Player hit Enemy!\n");
		}
	}

	// PlayerBullet vs Enemy
	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		for (PlayerBullet* bullet : player_->GetBullets()) {
			if (enemy->GetCollision().CheckCollision(bullet->GetCollision())) {
				enemy->TakeDamage(10);
				OutputDebugStringA("PlayerBullet hit Enemy!\n");
			}
		}
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

	if (stage_) {
		stage_->Draw(camera);
	}

	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
#pragma endregion
}
