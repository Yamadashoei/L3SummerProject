#include "PlayerBullet.h"

using namespace KamataEngine;

void PlayerBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	playerBulletModel = model;

	// 弾用の単色テクスチャを読み込み
	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	// ワールド変換初期化
	worldTransform_.Initialize();

	// 初期位置・速度をセット
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	// 初期マトリクス転送（見た目が一瞬出ないのを防止）
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update() {
	// 位置更新
	worldTransform_.translation_ += velocity_;

	// デスタイマー更新
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// マトリクス更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const KamataEngine::Camera& viewProjection) { playerBulletModel->Draw(worldTransform_, viewProjection, textureHandle_); }
