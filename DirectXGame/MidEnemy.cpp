#include "MidEnemy.h"
#include "kMath.h"

void MidEnemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f}; 

	worldTransform_.TransferMatrix();

	hp_ = 300;

	currentPhase_ = Phase::Phase1;
	attackTimer_ = kAttackInterval;
	phaseTimer_ = 600;
}

void MidEnemy::Update(const KamataEngine::Vector3& playerPosition) {
	phaseTimer_--;
	if (phaseTimer_ <= 0) {
		switch (currentPhase_) {
		case Phase::Phase1:
			currentPhase_ = Phase::Phase2;
			OutputDebugStringA("MidEnemy: Phase2!\n");
			break;
		case Phase::Phase2:
			currentPhase_ = Phase::Phase3;
			OutputDebugStringA("MidEnemy: Phase3!\n");
			break;
		case Phase::Phase3:
			currentPhase_ = Phase::Phase1;
			OutputDebugStringA("MidEnemy: Phase1!\n");
			break;
		}
		phaseTimer_ = 600;
	}

	Attack(playerPosition);

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
	collision_.SetRadius(2.0f);
}

void MidEnemy::Draw(const KamataEngine::Camera& camera) {
	model_->Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void MidEnemy::Attack(const KamataEngine::Vector3& playerPosition) {
	attackTimer_--;

	int interval = kAttackInterval;
	float bulletSpeed = 0.3f;

	switch (currentPhase_) {
	case Phase::Phase1:
		interval = 60;
		bulletSpeed = 0.3f;
		break;
	case Phase::Phase2:
		interval = 40;
		bulletSpeed = 0.5f;
		break;
	case Phase::Phase3:
		interval = 20;
		bulletSpeed = 0.8f;
		break;
	}

	if (attackTimer_ <= 0) {
		KamataEngine::Vector3 toPlayer = playerPosition - worldTransform_.translation_;
		KamataEngine::Vector3 dir = Normalize(toPlayer);
		KamataEngine::Vector3 velocity = dir * bulletSpeed;

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);

		attackTimer_ = interval;
	}
}
