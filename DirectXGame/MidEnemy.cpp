#include "MidEnemy.h"
#include "kMath.h"
#include <2d/Sprite.h>
#include <3d/Camera.h>
#include <algorithm>
#include <base/DirectXCommon.h>
#include <base/TextureManager.h>

using namespace KamataEngine;

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

	uint32_t whiteTex = TextureManager::Load("./Resources/white1x1.png");
	hpBar_.Initialize(whiteTex); 
}

void MidEnemy::Update(const KamataEngine::Vector3& playerPosition, const KamataEngine::Camera& camera) {
	// フェーズ切替
	phaseTimer_--;
	if (phaseTimer_ <= 0) {
		switch (currentPhase_) {
		case Phase::Phase1:
			currentPhase_ = Phase::Phase2;
			break;
		case Phase::Phase2:
			currentPhase_ = Phase::Phase3;
			break;
		case Phase::Phase3:
			currentPhase_ = Phase::Phase1;
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

	// HPバー更新
	hpBar_.Update(worldTransform_.translation_, camera, hp_, 300);
}

void MidEnemy::Draw(const KamataEngine::Camera& camera) {
	model_->Draw(worldTransform_, camera);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void MidEnemy::DrawHPBar() { hpBar_.Draw(); }

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
		Vector3 toPlayer = playerPosition - worldTransform_.translation_;
		Vector3 dir = Normalize(toPlayer);
		Vector3 velocity = dir * bulletSpeed;

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);

		attackTimer_ = interval;
	}
}
