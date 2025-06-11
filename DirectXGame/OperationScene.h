#pragma once
#include "KamataEngine.h"

class OperationScene {
public:
	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return nextSceneRequested_; }

private:
	KamataEngine::Sprite* operationSprite_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	bool nextSceneRequested_ = false;
};
