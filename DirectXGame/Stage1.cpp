#include "Stage1.h"
#include <Windows.h>
#include <cassert>
#include <cstdlib> // rand()

void Stage1::Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) {
	stageFinished_ = false;
	player_ = player;
	timer_ = 0;
	wave_ = 0;

	modelEnemy_ = modelEnemy;
	modelMidEnemy_ = modelMidEnemy;

	enemies_.clear();
}

void Stage1::Update(KamataEngine::Camera& camera) {
	timer_++;

	// Wave 0: 小型敵3体（ばらけて出現）
	if (wave_ == 0 && timer_ > 60) {
		for (int i = 0; i < 3; ++i) {
			float x = -8.0f + float(rand() % 17); // -8.0〜+8.0
			float y = -2.0f + float(rand() % 5);  // -2.0〜+2.0
			float z = 25.0f + float(rand() % 5);  // 25〜30

			auto enemy = new SmallEnemy();
			enemy->Initialize(modelEnemy_, {x, y, z}, SmallEnemy::AttackType::Shoot);
			AddEnemy(enemy);
		}
		wave_++;
	}

	// Wave 1: 小型敵5体（広がって出現）
	else if (wave_ == 1 && enemies_.empty()) {
		for (int i = 0; i < 5; ++i) {
			float x = -10.0f + float(rand() % 21); // -10〜+10
			float y = -3.0f + float(rand() % 7);   // -3〜+3
			float z = 27.0f + float(rand() % 6);   // 27〜33

			auto enemy = new SmallEnemy();
			enemy->Initialize(modelEnemy_, {x, y, z}, SmallEnemy::AttackType::Ram);
			AddEnemy(enemy);
		}
		wave_++;
	}

	// Wave 2: 中ボス1体出現
	else if (wave_ == 2 && enemies_.empty()) {
		auto mid = new MidEnemy();
		mid->Initialize(modelMidEnemy_, {0.0f, 0.0f, 30.0f});
		AddEnemy(mid);
		wave_++;
	}

	// Wave 3: 全撃破でクリア
	else if (wave_ == 3 && enemies_.empty()) {
		stageFinished_ = true;
		OutputDebugStringA("Stage1 Clear!\n");
	}

	// 敵更新（Camera を渡す）
	for (BaseEnemy* enemy : enemies_) {
		enemy->Update(player_->GetWorldPosition(), camera);
	}

	// 敵削除
	enemies_.remove_if([](BaseEnemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			OutputDebugStringA("Enemy destroyed (Stage1)\n");
			return true;
		}
		return false;
	});
}

void Stage1::Draw(KamataEngine::Camera& camera) {
	for (BaseEnemy* enemy : enemies_) {
		enemy->Draw(camera);
	}
}

bool Stage1::IsStageFinished() const { return stageFinished_; }

void Stage1::AddEnemy(BaseEnemy* enemy) {
	assert(enemy);
	enemies_.push_back(enemy);
}
