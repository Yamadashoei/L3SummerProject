#pragma once
#include "KamataEngine.h"

class RuleScene {
public:
	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return nextSceneRequested_; }

private:
	KamataEngine::Sprite* ruleSprite_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	bool nextSceneRequested_ = false;
};
