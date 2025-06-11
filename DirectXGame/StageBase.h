#pragma once
#include "BaseEnemy.h"
#include "Player.h"

#include <list>

class StageBase {
public:
	virtual ~StageBase() = default;

	virtual void Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) = 0;
	virtual void Update() = 0;
	virtual void Draw(KamataEngine::Camera& camera) = 0;
	virtual bool IsStageFinished() const = 0;

	const std::list<BaseEnemy*>& GetEnemies() const { return enemies_; }

protected:
	std::list<BaseEnemy*> enemies_;
};
