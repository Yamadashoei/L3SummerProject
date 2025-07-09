#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include "kMath.h"
#include <cassert>

using namespace KamataEngine;

class PlayerBullet {

public:
	// 初期化
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw(const KamataEngine::Camera& viewProjection);

	// デスフラグ getter
	bool IsDead() const { return isDead_; }

	const Collision& GetCollision() const { return collision_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerBulletModel = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0u;

	// 速度
	KamataEngine::Vector3 velocity_;

	// 寿命（フレーム単位）
	static const int32_t kLifeTime = 60 * 5; // 5秒

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

	// デスフラグ
	bool isDead_ = false;

	Collision collision_;
};
