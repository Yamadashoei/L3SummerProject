#include "GameScene.h"
#include <base/DirectXCommon.h>
#include <base/TextureManager.h>
#include <base/WinApp.h>
#include <cassert>
#include "BigEnemy.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelMidEnemy_;
	delete modelBigEnemy_;
	delete player_;
	delete debugCamera_;
	delete hpBackSprite_;
	delete hpBarSprite_;
	delete reticle_;

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
	modelEnemy_ = Model::CreateFromOBJ("cube");
	modelMidEnemy_ = Model::CreateFromOBJ("cube");
	modelBigEnemy_ = Model::CreateFromOBJ("cube");

	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&camera);

	delete stage_;
	stage_ = new Stage1();
	stage_->Initialize(modelEnemy_, modelMidEnemy_, player_);

	uint32_t whiteTex = TextureManager::Load("./Resources/white1x1.png");
	Vector2 barPos = {20.0f, 20.0f};
	hpBackSprite_ = Sprite::Create(whiteTex, barPos, {0.3f, 0.3f, 0.3f, 1.0f});
	hpBackSprite_->SetSize({200.0f, 20.0f});
	hpBarSprite_ = Sprite::Create(whiteTex, barPos, {1.0f, 0.0f, 0.0f, 1.0f});
	hpBarSprite_->SetSize({200.0f, 20.0f});

	// レティクル初期化
	uint32_t reticleTex = TextureManager::Load("./Resources/reticle.png");
	reticle_ = Sprite::Create(reticleTex, {640.0f, 360.0f});
	reticle_->SetAnchorPoint({0.5f, 0.5f});

	scoreManager_ = new ScoreManager();
	scoreManager_->Initialize();

	uint32_t numberTex = TextureManager::Load("./Resources/number.png");
	scoreUI_ = new ScoreUI();
	scoreUI_->Initialize(numberTex, {1000.0f, 20.0f}); // 右上表示など


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

	if (stage_) {
		stage_->Update(camera);
	}

	// ===== 当たり判定処理 =====
	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		if (player_->GetCollision().CheckCollision(enemy->GetCollision())) {
			player_->TakeDamage(20);
			player_->SetHit();
			OutputDebugStringA("Player hit Enemy!\n");
		}
	}

	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		for (PlayerBullet* bullet : player_->GetBullets()) {
			if (enemy->GetCollision().CheckCollision(bullet->GetCollision())) {
				enemy->TakeDamage(10);
				OutputDebugStringA("PlayerBullet hit Enemy!\n");
			}
		}
	}

	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		for (EnemyBullet* bullet : enemy->GetBullets()) {
			if (player_->GetCollision().CheckCollision(bullet->GetCollision())) {
				player_->TakeDamage(10);
				player_->SetHit();
				OutputDebugStringA("EnemyBullet hit Player!\n");
			}
		}
	}

	if (player_->IsDead()) {
		OutputDebugStringA("PLAYER DEAD - Game Over\n");
		PostQuitMessage(0);
	}

	// レティクル位置更新
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePos);
	Vector2 reticlePos = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
	reticle_->SetPosition(reticlePos);

	for (BaseEnemy* e : stage_->GetEnemies()) {
		if (e->IsDead()) {
			if (dynamic_cast<MidEnemy*>(e)) {
				scoreManager_->AddScore(1000);
			} else if (dynamic_cast<BigEnemy*>(e)) {
				scoreManager_->AddScore(5000);
			} else {
				int r = rand() % 201 + 100; // 100〜300点
				scoreManager_->AddScore(r);
			}
		}
	}


}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);

	for (BaseEnemy* enemy : stage_->GetEnemies()) {
		if (auto* mid = dynamic_cast<MidEnemy*>(enemy)) {
			mid->DrawHPBar();
		}
	}

	if (hpBackSprite_ && hpBarSprite_) {
		float hpRatio = static_cast<float>(player_->GetHP()) / player_->GetMaxHP();
		hpBarSprite_->SetSize({200.0f * hpRatio, 20.0f});
		hpBackSprite_->Draw();
		hpBarSprite_->Draw();
	}
	// レティクル描画
	if (reticle_) {
		reticle_->Draw();
	}
	// スコアUI描画
	scoreUI_->Draw(scoreManager_->GetScore());

	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
	Model::PreDraw(commandList);
	player_->Draw(camera);

	if (stage_) {
		stage_->Draw(camera);
	}

	Model::PostDraw();
}