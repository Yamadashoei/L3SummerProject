#pragma once
#include "BaseEnemy.h"
#include "EnemyBullet.h"
#include "EnemyHpBar.h"

namespace KamataEngine {
class Camera;
}

class MidEnemy : public BaseEnemy {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) override;
	void Update(const KamataEngine::Vector3& playerPosition, const KamataEngine::Camera& camera) override;
	void Draw(const KamataEngine::Camera& camera) override;
	void DrawHPBar(); // ← 追加

private:
	enum class Phase { Phase1, Phase2, Phase3 };
	void Attack(const KamataEngine::Vector3& playerPosition);

	Phase currentPhase_ = Phase::Phase1;
	std::list<EnemyBullet*> bullets_;

	int attackTimer_ = 0;
	static const int kAttackInterval = 60;
	int phaseTimer_ = 600;

	EnemyHpBar hpBar_; // ← 置き換え済み
};
