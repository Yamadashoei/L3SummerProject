#define NOMINMAX
#include <Windows.h>

#include "Player.h"
#include "PlayerBullet.h"
#include "kMath.h"
#include <algorithm>
#include <cassert>

#include <base/TextureManager.h>
#include <base/WinApp.h>
#include <input\Input.h>
#include <math\MathUtility.h>

using namespace KamataEngine;

Player::~Player() {
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	playerBullets_.clear();
}

void Player::Initialize(Model* model, Camera* camera) {
	assert(model);
	playerModel = model;
	camera_ = camera;
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.TransferMatrix();

	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(1.0f);
}

void Player::Update() {
	// 攻撃処理
	Attack();

	// 弾の更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	playerBullets_.remove_if([](PlayerBullet* b) {
		if (b->IsDead()) {
			delete b;
			return true;
		}
		return false;
	});

	// 自機の行動（仮でWASD移動）
	if (input_->PushKey(DIK_W))
		worldTransform_.translation_.y += 0.5f;
	if (input_->PushKey(DIK_S))
		worldTransform_.translation_.y -= 0.5f;
	if (input_->PushKey(DIK_A))
		worldTransform_.translation_.x -= 0.5f;
	if (input_->PushKey(DIK_D))
		worldTransform_.translation_.x += 0.5f;

	// マトリクス更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	collision_.SetPosition(worldTransform_.translation_);

	if (isHit_) {
		hitEffectTimer_--;
		if (hitEffectTimer_ <= 0) {
			isHit_ = false;
		}
	}
}

void Player::Draw(Camera& viewProjection) {
	playerModel->Draw(worldTransform_, viewProjection);

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::TakeDamage(int damage) {
	hp_ -= damage;
	hp_ = std::max(hp_, 0);
}

void Player::SetHit() {
	isHit_ = true;
	hitEffectTimer_ = 20;
}

void Player::Attack() {
	if (input_->IsTriggerMouse(0)) {
		// マウス位置取得
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePos);

		Vector2 screenPos = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
		// カメラからの距離に応じてZ深度を指定（一般的に1.0fでOK）
		float zDepth = worldTransform_.translation_.z + 10.0f;
		
		// ビュー・プロジェクション行列を合成して逆行列を取得
		Matrix4x4 viewProj = Multiply(camera_->matView, camera_->matProjection);
		Matrix4x4 invViewProj = KamataEngine::MathUtility::Inverse(viewProj);

		// スクリーン座標をワールド座標に変換
		Vector3 targetWorldPos = ScreenToWorld(screenPos, zDepth, camera_->matView, camera_->matProjection, WinApp::kWindowWidth, WinApp::kWindowHeight);

		// 自機の位置からターゲットへの方向ベクトル
		Vector3 toTarget = targetWorldPos - worldTransform_.translation_;
		Vector3 dir = Normalize(toTarget);
		Vector3 velocity = dir * 1.0f;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(playerModel, worldTransform_.translation_, velocity);
		playerBullets_.push_back(newBullet);
	}
}
