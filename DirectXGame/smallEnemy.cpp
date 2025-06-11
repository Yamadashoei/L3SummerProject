#include "SmallEnemy.h"
#include "kMath.h"

void SmallEnemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) { Initialize(model, position, AttackType::Shoot); }

void SmallEnemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.TransferMatrix();

	startPosition_ = position;
	attackType_ = type;

	hp_ = 10;
	attackTimer_ = kAttackInterval;
}

void SmallEnemy::Update(const KamataEngine::Vector3& playerPosition) {
	if (attackType_ == AttackType::Ram) {
		if (!isReturning_) {
			KamataEngine::Vector3 toPlayer = playerPosition - worldTransform_.translation_;
			float distance = Vector3Length(toPlayer);

			float hitDistance = collision_.GetRadius() + 1.0f;

			if (distance > hitDistance) {
				KamataEngine::Vector3 dir = Normalize(toPlayer);
				worldTransform_.translation_ += dir * 0.05f;
			} else {
				isReturning_ = true;
				OutputDebugStringA("RamEnemy starts returning!\n");
			}
		} else {
			KamataEngine::Vector3 toStart = startPosition_ - worldTransform_.translation_;
			float distanceToStart = Vector3Length(toStart);

			if (distanceToStart > 0.1f) {
				KamataEngine::Vector3 dir = Normalize(toStart);
				worldTransform_.translation_ += dir * 0.05f;
			} else {
				worldTransform_.translation_ = startPosition_;
				isReturning_ = false;
				OutputDebugStringA("RamEnemy return complete!\n");
			}
		}
	} else if (attackType_ == AttackType::Shoot) {
		Attack(playerPosition);
	}

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

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(1.0f);
}

void SmallEnemy::Draw(const KamataEngine::Camera& camera) {
	model_->Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void SmallEnemy::Attack(const KamataEngine::Vector3& playerPosition) {
	if (--attackTimer_ <= 0) {
		KamataEngine::Vector3 toPlayer = playerPosition - worldTransform_.translation_;
		KamataEngine::Vector3 dir = Normalize(toPlayer);
		KamataEngine::Vector3 velocity = dir * 0.3f;

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);

		attackTimer_ = kAttackInterval;
	}
}
