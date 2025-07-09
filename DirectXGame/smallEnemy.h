#pragma once
#include "BaseEnemy.h"
#include "EnemyBullet.h"

class SmallEnemy : public BaseEnemy {
public:
	enum class AttackType { Shoot, Ram };

	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) override;
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type);

	// Camera 引数付きに変更（BaseEnemyと一致させる）
	void Update(const KamataEngine::Vector3& playerPosition, const KamataEngine::Camera& camera) override;

	void Draw(const KamataEngine::Camera& camera) override;

	const std::list<EnemyBullet*>& GetBullets() const override { return bullets_; }

private:
	void Attack(const KamataEngine::Vector3& playerPosition);

	AttackType attackType_;
	KamataEngine::Vector3 startPosition_;
	bool isReturning_ = false;

	std::list<EnemyBullet*> bullets_;

	int attackTimer_ = 0;
	static const int kAttackInterval = 60;
};
