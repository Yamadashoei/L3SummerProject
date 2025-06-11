#pragma once
#include "MidEnemy.h"
#include "SmallEnemy.h"
#include "StageBase.h"

class Stage1 : public StageBase {
public:
	void Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) override;
	void Update() override;
	void Draw(KamataEngine::Camera& camera) override;
	bool IsStageFinished() const override;

private:
	bool stageFinished_ = false;

	Player* player_ = nullptr; // 参照用
};
