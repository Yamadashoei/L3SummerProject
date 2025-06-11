#include "Stage1.h"
#include <Windows.h>

void Stage1::Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) {
	stageFinished_ = false;
	player_ = player;

	// ★ SmallEnemy生成
	SmallEnemy* shootEnemy = new SmallEnemy();
	shootEnemy->Initialize(modelEnemy, {0.0f, 5.0f, 10.0f}, SmallEnemy::AttackType::Shoot);
	enemies_.push_back(shootEnemy);

	SmallEnemy* ramEnemy = new SmallEnemy();
	ramEnemy->Initialize(modelEnemy, {-5.0f, 3.0f, 20.0f}, SmallEnemy::AttackType::Ram);
	enemies_.push_back(ramEnemy);

	// ★ MidEnemy生成
	MidEnemy* midEnemy = new MidEnemy();
	midEnemy->Initialize(modelMidEnemy, {5.0f, 0.0f, 15.0f});
	enemies_.push_back(midEnemy);

	// BigEnemyは後で同様に追加OK
}

void Stage1::Update() {
	for (BaseEnemy* enemy : enemies_) {
		enemy->Update(player_->GetWorldPosition());
	}

	// Enemy削除
	enemies_.remove_if([](BaseEnemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			OutputDebugStringA("Enemy destroyed (Stage1)!\n");
			return true;
		}
		return false;
	});

	// ステージ終了条件（例：敵が全滅したらクリア）
	if (enemies_.empty()) {
		stageFinished_ = true;
	}
}

void Stage1::Draw(KamataEngine::Camera& camera) {
	for (BaseEnemy* enemy : enemies_) {
		enemy->Draw(camera);
	}
}

bool Stage1::IsStageFinished() const { return stageFinished_; }
