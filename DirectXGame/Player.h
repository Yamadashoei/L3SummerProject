#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include "PlayerBullet.h"

class Player {

public:
	// デストラクタ
	~Player();

	// 初期化
	void Initialize(KamataEngine::Model* model);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& viewProjection);

	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }

	const Collision& GetCollision() const { return collision_; }
	const std::list<PlayerBullet*>& GetBullets() const { return playerBullets_; } // 既にあるかもしれない

	int GetHP() const { return hp_; }
	void TakeDamage(int damage) {
		hp_ -= damage;
		if (hp_ < 0)
			hp_ = 0;
	}
	bool IsDead() const { return hp_ <= 0; }


private:
	// 発射
	void Attack();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel = nullptr;

	KamataEngine::Input* input_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;

	Collision collision_;

	int hp_ = 100; // プレイヤー初期HP 100
};
