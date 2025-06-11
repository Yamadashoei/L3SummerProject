#pragma once
#include "KamataEngine.h"
#include "kMath.h"

#include <cassert>

using namespace KamataEngine;

class EnemyBullet {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(const KamataEngine::Camera& camera);

	// デスフラグ
	bool IsDead() const { return isDead_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* enemyBulletModel_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_ = 0u;

	// 速度
	KamataEngine::Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5; // 5秒間

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};
