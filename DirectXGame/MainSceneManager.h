#pragma once
#include "GameScene.h"
#include "OperationScene.h"
#include "RuleScene.h"
#include "TitleScene.h"

enum class SceneState { Title, Rule, Operation, Game };

class MainSceneManager {
public:
	void Initialize();
	void Update();
	void Draw();

	void Finalize();


private:
	SceneState currentScene_ = SceneState::Title;

	TitleScene* titleScene_ = nullptr;
	RuleScene* ruleScene_ = nullptr;
	OperationScene* operationScene_ = nullptr;
	GameScene* gameScene_ = nullptr;
};
