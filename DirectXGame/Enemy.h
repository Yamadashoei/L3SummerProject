#pragma once
#include "EnemyBullet.h"
#include "KamataEngine.h"

#include <list>

class Enemy {
public:
	// 攻撃タイプ
	enum class AttackType { None, Shoot, Ram };

public:
	~Enemy();

	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type);

	void Update();

	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_; }

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	// ★ プレイヤー位置をセット
	void SetPlayerPosition(const KamataEngine::Vector3& playerPos) { playerPosition_ = playerPos; }

private:
	void Move();
	void Attack();

private:
	float moveRangeX_ = 5.0f;
	float moveSpeedX_ = 0.1f;
	int moveDir_ = 1;

	KamataEngine::Model* enemyModel = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	AttackType attackType_ = AttackType::None;

	bool isDead_ = false;

	int attackTimer_ = 0;
	const int kAttackInterval = 60;

	std::list<EnemyBullet*> bullets_;

	// ★ プレイヤー位置保持用
	KamataEngine::Vector3 playerPosition_{};
};
