#include "Enemy.h"
#include "kMath.h"

void Enemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, AttackType type, EnemyType enemyType) {
	enemyModel = model;
	attackType_ = type;
	type_ = enemyType;
	startPosition_ = position; // 初期位置を記録


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.TransferMatrix();

	// HP初期化
	switch (type_) {
	case EnemyType::Fish:
	case EnemyType::Mine:
		hp_ = 10;
		break;
	case EnemyType::Jellyfish:
	case EnemyType::Eel:
		hp_ = 20;
		break;
	case EnemyType::MidBoss:
		hp_ = 300;
		break;
	case EnemyType::KrakenTentacle:
		hp_ = 75;
		break;
	case EnemyType::KrakenCore:
		hp_ = 600;
		break;
	}

	attackTimer_ = kAttackInterval;
}

void Enemy::Update(const KamataEngine::Vector3& playerPosition) {
	if (attackType_ == AttackType::Ram) {
		if (!isReturning_) {
			// 通常突撃モード
			KamataEngine::Vector3 toPlayer = playerPosition - worldTransform_.translation_;
			float distance = Vector3Length(toPlayer);

			float hitDistance = collision_.GetRadius() + 1.0f; // プレイヤー半径仮1.0f

			if (distance > hitDistance) {
				// 追尾移動
				KamataEngine::Vector3 dir = Normalize(toPlayer);
				worldTransform_.translation_ += dir * 0.05f;
			} else {
				// 当たったら → 戻り状態に切り替え
				isReturning_ = true;
				OutputDebugStringA("RamEnemy starts returning!\n");
			}
		} else {
			// 戻りモード
			KamataEngine::Vector3 toStart = startPosition_ - worldTransform_.translation_;
			float distanceToStart = Vector3Length(toStart);

			if (distanceToStart > 0.1f) {
				KamataEngine::Vector3 dir = Normalize(toStart);
				worldTransform_.translation_ += dir * 0.05f;
			} else {
				// 戻り完了 → 通常モードに戻す
				worldTransform_.translation_ = startPosition_;
				isReturning_ = false;
				OutputDebugStringA("RamEnemy return complete!\n");
			}
		}
	}




	// 攻撃
	Attack(playerPosition);

	// Bullets更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// Bullets削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// WorldTransform更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// Collision更新
	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(1.0f); // 調整可
}

void Enemy::Draw(const KamataEngine::Camera& camera) {
	enemyModel->Draw(worldTransform_, camera);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

void Enemy::Attack(const KamataEngine::Vector3& playerPosition) {
	if (attackType_ == AttackType::Shoot) {
		if (--attackTimer_ <= 0) {
			KamataEngine::Vector3 toPlayer = playerPosition - worldTransform_.translation_;
			KamataEngine::Vector3 dir = Normalize(toPlayer);
			KamataEngine::Vector3 velocity = dir * 0.3f;

			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(enemyModel, worldTransform_.translation_, velocity);
			bullets_.push_back(newBullet);

			attackTimer_ = kAttackInterval;
		}
	}
}
