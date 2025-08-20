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
	if (wave_ == 0 && timer_ > kWave0StartFrame) {
		SpawnSmallEnemies_Shoot(kWave0SmallCount);
		wave_++;
	}
	// Wave 1: 小型敵5体（広がって出現）
	else if (wave_ == 1 && enemies_.empty()) {
		SpawnSmallEnemies_Ram(kWave1SmallCount);
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
	if (!enemy)
		return; // ★ 保険
	enemies_.push_back(enemy);
}

// ------- 追加：スポーン補助（従来と同じ分布になるようにしてある） -------
void Stage1::SpawnSmallEnemies_Shoot(int count) {
	for (int i = 0; i < count; ++i) {
		float x = RandLikeIntSteps(kW0XMin, kW0XSpan, 16); // -8..+8 → 17ステップ
		float y = RandLikeIntSteps(kW0YMin, kW0YSpan, 4);  // -2..+2 → 5ステップ
		float z = RandLikeIntSteps(kW0ZMin, kW0ZSpan, 4);  // 25..30 → 5ステップ
		auto e = new SmallEnemy();
		e->Initialize(modelEnemy_, {x, y, z}, SmallEnemy::AttackType::Shoot);
		AddEnemy(e);
	}
}

void Stage1::SpawnSmallEnemies_Ram(int count) {
	for (int i = 0; i < count; ++i) {
		float x = RandLikeIntSteps(kW1XMin, kW1XSpan, 20); // -10..+10 → 21ステップ
		float y = RandLikeIntSteps(kW1YMin, kW1YSpan, 6);  // -3..+3   → 7ステップ
		float z = RandLikeIntSteps(kW1ZMin, kW1ZSpan, 6);  // 27..33   → 7ステップ
		auto e = new SmallEnemy();
		e->Initialize(modelEnemy_, {x, y, z}, SmallEnemy::AttackType::Ram);
		AddEnemy(e);
	}
}
