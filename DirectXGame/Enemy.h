#pragma once
#include "Collision.h"
#include "EnemyBullet.h"
#include "KamataEngine.h"

class Enemy {
public:
	enum class AttackType { Shoot, Ram };

	enum class EnemyType { Fish, Jellyfish, Eel, Mine, MidBoss, KrakenTentacle, KrakenCore };

	// 初期化
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type, EnemyType enemyType);

	// 更新
	void Update(const KamataEngine::Vector3& playerPosition);

	// 描画
	void Draw(const KamataEngine::Camera& camera);

	// 弾取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	// Collision取得
	const Collision& GetCollision() const { return collision_; }

	// HP
	int GetHP() const { return hp_; }
	void TakeDamage(int damage) {
		hp_ -= damage;
		if (hp_ < 0)
			hp_ = 0;
	}
	bool IsDead() const { return hp_ <= 0; }

	// EnemyType取得
	EnemyType GetEnemyType() const { return type_; }

private:
	void Attack(const KamataEngine::Vector3& playerPosition);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* enemyModel = nullptr;

	AttackType attackType_;
	EnemyType type_;
	int hp_ = 10;

	Collision collision_;

	std::list<EnemyBullet*> bullets_;

	int attackTimer_ = 0;
	static const int kAttackInterval = 60;

	KamataEngine::Vector3 startPosition_; // 体当たり時に戻る初期位置
	bool isReturning_ = false;            // 今戻り中かどうか
};
