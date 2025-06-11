#pragma once
#include "KamataEngine.h"

enum class SceneState;

class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return nextSceneRequested_; }
	SceneState GetNextScene() const { return nextScene_; }

private:
	KamataEngine::Sprite* titleSprite_ = nullptr;
	KamataEngine::Input* input_ = nullptr;

	bool nextSceneRequested_ = false;
	SceneState nextScene_;

	// ★ 追加：Scene切替時のキー入力スキップ用タイマー
	int changeWaitTimer_ = 0;
};
