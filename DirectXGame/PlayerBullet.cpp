#include "PlayerBullet.h"

using namespace KamataEngine;

void PlayerBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	assert(model);
	playerBulletModel = model;

	// テクスチャ読み込み（単色白）
	textureHandle_ = TextureManager::Load("./Resources/PlayerBullet/playerBullet.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update() {
	// 位置更新
	worldTransform_.translation_ += velocity_;

	// デスタイマー減少
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 当たり判定更新
	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(0.5f);
}

void PlayerBullet::Draw(const KamataEngine::Camera& viewProjection) { playerBulletModel->Draw(worldTransform_, viewProjection, textureHandle_); }
