#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"

#include <cassert>

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete player_;
	delete modelEnemy_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

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
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("cube");

	// プレイヤー生成・初期化
	player_ = new Player();
	player_->Initialize(modelPlayer_);

	// Enemy モデル読み込み
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube");

	// 弾撃ち敵の追加
	Enemy* shootEnemy = new Enemy();
	shootEnemy->Initialize(modelEnemy_, {0.0f, 5.0f, 10.0f}, Enemy::AttackType::Shoot, Enemy::EnemyType::Fish);
	enemies_.push_back(shootEnemy);

	// 体当たり敵の追加（動きはまだ未実装だけど先に配置だけOK）
	Enemy* ramEnemy = new Enemy();
	ramEnemy->Initialize(modelEnemy_, {-5.0f, 3.0f, 20.0f}, Enemy::AttackType::Ram, Enemy::EnemyType::Fish);
	enemies_.push_back(ramEnemy);

	// デバッグカメラ生成
	debugCamera_ = new KamataEngine::DebugCamera(KamataEngine::WinApp::kWindowWidth, KamataEngine::WinApp::kWindowHeight);

	// 軸表示
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&camera);
}

void GameScene::Update() {

	// ★ プレイヤー位置取得
	Vector3 playerPos = player_->GetWorldPosition();

	// ★ 各Enemyにプレイヤー位置をセット
	for (Enemy* enemy : enemies_) {
		//enemy->SetPlayerPosition(playerPos);
		enemy->Update(playerPos);
	}

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

	// 敵更新
	for (Enemy* enemy : enemies_) {
		enemy->Update(player_->GetWorldPosition());
	}

	// ① Player と Enemy の当たり判定
	for (Enemy* enemy : enemies_) {
		if (player_->GetCollision().CheckCollision(enemy->GetCollision())) {
			player_->TakeDamage(20); // 敵体当たり → プレイヤーに20ダメージ
			OutputDebugStringA("Player hit Enemy!\n");
		}
	}

	// ② Player と EnemyBullet の当たり判定
	for (Enemy* enemy : enemies_) {
		for (EnemyBullet* bullet : enemy->GetBullets()) {
			if (player_->GetCollision().CheckCollision(bullet->GetCollision())) {
				player_->TakeDamage(10); // 弾ヒット → プレイヤーに10ダメージ
				OutputDebugStringA("Player hit EnemyBullet!\n");
			}
		}
	}

	// ③ PlayerBullet と Enemy の当たり判定
	for (Enemy* enemy : enemies_) {
		for (PlayerBullet* bullet : player_->GetBullets()) {
			if (enemy->GetCollision().CheckCollision(bullet->GetCollision())) {
				enemy->TakeDamage(10); // プレイヤー弾 → 敵に10ダメージ
				OutputDebugStringA("PlayerBullet hit Enemy!\n");
			}
		}
	}

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			OutputDebugStringA("Enemy destroyed!\n");
			return true;
		}
		return false;
	});

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

	// プレイヤー描画
	player_->Draw(camera);

	// 敵描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera);
	}

	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
#pragma endregion
}
