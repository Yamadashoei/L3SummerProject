#pragma once
#include "MidEnemy.h"
#include "SmallEnemy.h"
#include "StageBase.h"

class Stage1 : public StageBase {
public:
	void Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) override;

	// Camera 引数を追加（BaseEnemy::Update 対応）
	void Update(KamataEngine::Camera& camera) override;

	void Draw(KamataEngine::Camera& camera) override;
	bool IsStageFinished() const override;

private:
	bool stageFinished_ = false;
	Player* player_ = nullptr;

	int wave_ = 0;
	int timer_ = 0;

	// 敵モデル保存（再出現用）
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelMidEnemy_ = nullptr;

	// 敵追加補助
	void AddEnemy(BaseEnemy* enemy);
};
