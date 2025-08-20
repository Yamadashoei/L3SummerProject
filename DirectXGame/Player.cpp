#define NOMINMAX
#include <Windows.h>

#include "Player.h"
#include "PlayerBullet.h"
#include "kMath.h"
#include <algorithm>
#include <cassert>
#include <cmath>

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
	worldTransform_.translation_ = {0.0f, 0.0f, -20.0f};
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

	// ===== スムーズ移動（加減速＋斜め正規化） =====
	KamataEngine::Vector2 in = {0.0f, 0.0f};
	if (input_->PushKey(DIK_W))
		in.y += 1.0f;
	if (input_->PushKey(DIK_S))
		in.y -= 1.0f;
	if (input_->PushKey(DIK_A))
		in.x -= 1.0f;
	if (input_->PushKey(DIK_D))
		in.x += 1.0f;

	if (in.x != 0.0f || in.y != 0.0f) {
		float len = std::sqrt(in.x * in.x + in.y * in.y);
		in.x /= len;
		in.y /= len;
	}

	KamataEngine::Vector3 targetVel = {in.x * move_.maxSpeed, in.y * move_.maxSpeed, 0.0f};

	auto approach = [](float cur, float target, float accel, float decel) {
		bool accelerating = std::abs(target) > std::abs(cur);
		float step = accelerating ? accel : decel;
		float diff = target - cur;
		if (diff > step)
			return cur + step;
		if (diff < -step)
			return cur - step;
		return target;
	};
	velocity_.x = approach(velocity_.x, targetVel.x, move_.accel, move_.decel);
	velocity_.y = approach(velocity_.y, targetVel.y, move_.accel, move_.decel);

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;

	// 行列更新
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
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePos);

		Vector2 screenPos = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
		float zDepth = worldTransform_.translation_.z + 10.0f;

		Matrix4x4 viewProj = Multiply(camera_->matView, camera_->matProjection);
		Matrix4x4 invViewProj = KamataEngine::MathUtility::Inverse(viewProj);

		Vector3 targetWorldPos = ScreenToWorld(screenPos, zDepth, camera_->matView, camera_->matProjection, WinApp::kWindowWidth, WinApp::kWindowHeight);

		Vector3 toTarget = targetWorldPos - worldTransform_.translation_;
		Vector3 dir = Normalize(toTarget * -1.0f); // 既存仕様（反転）を維持
		Vector3 velocity = dir * 1.0f;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(playerModel, worldTransform_.translation_, velocity);
		playerBullets_.push_back(newBullet);
	}
}

void Player::SetReticlePosition(const KamataEngine::Vector2& pos) { reticlePosition_ = pos; }

// ★ GameScene から呼ぶ可視範囲クランプ
void Player::ClampPositionXY(float minX, float maxX, float minY, float maxY) {
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, minX, maxX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, minY, maxY);
	// Transform と当たり判定を同期
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	collision_.SetPosition(worldTransform_.translation_);
}
