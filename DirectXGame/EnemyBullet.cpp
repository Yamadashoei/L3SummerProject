#include "EnemyBullet.h"

using namespace KamataEngine;

void EnemyBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	assert(model);

	enemyBulletModel_ = model;

	// ここは別の見た目にしたければ "redBullet.png" など別のテクスチャを指定してOK
	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	worldTransform_.TransferMatrix();
}

void EnemyBullet::Update() {
	// 移動
	worldTransform_.translation_ += velocity_;

	// 寿命カウント
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// マトリクス更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(0.5f);

}

void EnemyBullet::Draw(const KamataEngine::Camera& camera) { enemyBulletModel_->Draw(worldTransform_, camera, textureHandle_); }
