#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	~Player();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw(KamataEngine::Camera& viewProjection);

	void TakeDamage(int damage);
	bool IsDead() const { return hp_ <= 0; }

	int GetHP() const { return hp_; }
	int GetMaxHP() const { return maxHp_; }

	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }
	const Collision& GetCollision() const { return collision_; }
	const std::list<PlayerBullet*>& GetBullets() const { return playerBullets_; }

	void SetHit();

	// レティクルのスクリーン座標を受け取る
	void SetReticlePosition(const KamataEngine::Vector2& pos);

private:
	void Attack();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;
	Collision collision_;

	int hp_ = 100;
	const int maxHp_ = 100;

	bool isHit_ = false;
	int hitEffectTimer_ = 0;

	KamataEngine::Vector2 reticlePosition_ = {640.0f / 2, 360.0f / 2}; // デフォルトで画面中央
};