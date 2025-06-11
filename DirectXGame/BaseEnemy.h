#pragma once
#include "Collision.h"
#include "KamataEngine.h"

class BaseEnemy {
public:
	virtual ~BaseEnemy() = default;

	virtual void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) = 0;
	virtual void Update(const KamataEngine::Vector3& playerPosition) = 0;
	virtual void Draw(const KamataEngine::Camera& camera) = 0;

	void TakeDamage(int damage);
	bool IsDead() const { return hp_ <= 0; }

	const Collision& GetCollision() const { return collision_; }

protected:
	int hp_ = 0;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	Collision collision_;
};
