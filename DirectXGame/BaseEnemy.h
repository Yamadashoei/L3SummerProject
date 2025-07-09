#pragma once
#include "Collision.h"
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include <list>

class BaseEnemy {
public:
	virtual ~BaseEnemy() = default;

	virtual void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) = 0;

	// ✅ Cameraを引数に追加
	virtual void Update(const KamataEngine::Vector3& playerPosition, const KamataEngine::Camera& camera) = 0;

	virtual void Draw(const KamataEngine::Camera& camera) = 0;

	void TakeDamage(int damage) { hp_ -= damage; }
	bool IsDead() const { return hp_ <= 0; }

	const Collision& GetCollision() const { return collision_; }

	// 敵の弾取得用
	virtual const std::list<EnemyBullet*>& GetBullets() const {
		static std::list<EnemyBullet*> empty;
		return empty;
	}

protected:
	int hp_ = 0;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	Collision collision_;
};
