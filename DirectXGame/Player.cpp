#include "Player.h"
#include "PlayerBullet.h"
#include "kMath.h"

#include <cassert>
#include <algorithm>

using namespace KamataEngine;

Player::~Player() {
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

void Player::Initialize(KamataEngine::Model* model) {
	assert(model);

	playerModel = model;

	// WorldTransform 初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, -20.0f}; // TPS向け初期位置
	worldTransform_.TransferMatrix();

	input_ = KamataEngine::Input::GetInstance();
}

void Player::Update() {
	// 弾の削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 移動処理
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}

	worldTransform_.translation_ += move;


	// 移動限界
	const float kMoveLimitX = 10.0f;
	const float kMoveLimitY = 10.0f;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, +kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, +kMoveLimitY);

	// 回転
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// マトリクス更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 攻撃
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(1.0f);

}

void Player::Draw(KamataEngine::Camera& viewProjection) {
	playerModel->Draw(worldTransform_, viewProjection);

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(playerModel, worldTransform_.translation_, velocity);

		playerBullets_.push_back(newBullet);
	}
}
