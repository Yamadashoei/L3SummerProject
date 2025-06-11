#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"

#include <list>

class Player {
public:
	~Player();

	void Initialize(KamataEngine::Model* model);
	void Update();
	void Draw(KamataEngine::Camera& viewProjection);

	void TakeDamage(int damage);
	bool IsDead() const { return hp_ <= 0; }

	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }
	const Collision& GetCollision() const { return collision_; }
	const std::list<PlayerBullet*>& GetBullets() const { return playerBullets_; }

	void SetHit(); // ★ 追加：ヒット時のエフェクト用

private:
	void Attack();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel = nullptr;

	KamataEngine::Input* input_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;

	Collision collision_;

	int hp_ = 100; // 初期HP

	// ★ ヒット時エフェクト用
	bool isHit_ = false;
	int hitEffectTimer_ = 0;
};
