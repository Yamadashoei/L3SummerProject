#include "Enemy.h"
#include "kMath.h"

using namespace KamataEngine;

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type) {
	enemyModel = model;
	attackType_ = type;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.TransferMatrix();

	attackTimer_ = kAttackInterval;
}

void Enemy::Update() {
	Move();
	Attack();

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}

void Enemy::Draw(const KamataEngine::Camera& camera) {
	enemyModel->Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Enemy::Move() {
	worldTransform_.translation_.x += moveSpeedX_ * moveDir_;

	if (worldTransform_.translation_.x > moveRangeX_) {
		worldTransform_.translation_.x = moveRangeX_;
		moveDir_ = -1;
	}
	if (worldTransform_.translation_.x < -moveRangeX_) {
		worldTransform_.translation_.x = -moveRangeX_;
		moveDir_ = 1;
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Attack() {
	if (attackType_ == AttackType::Shoot) {
		if (--attackTimer_ <= 0) {
			const float kBulletSpeed = -0.5f;
			Vector3 velocity(0, 0, kBulletSpeed);
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(enemyModel, worldTransform_.translation_, velocity);

			bullets_.push_back(newBullet);

			attackTimer_ = kAttackInterval;
		}
	} else if (attackType_ == AttackType::Ram) {
		// ★ 追尾ロジック
		const float kRamSpeed = 0.05f;
		Vector3 toPlayer = playerPosition_ - worldTransform_.translation_;
		float distance = Vector3Length(toPlayer);

		if (distance > 0.1f) {
			Vector3 dir =  Normalize(toPlayer);
			worldTransform_.translation_ += dir * kRamSpeed;
		}
	}
}
