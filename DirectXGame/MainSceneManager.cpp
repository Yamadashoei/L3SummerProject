#include "MainSceneManager.h"

void MainSceneManager::Initialize() {
	titleScene_ = new TitleScene();
	ruleScene_ = new RuleScene();
	operationScene_ = new OperationScene();
	gameScene_ = new GameScene();

	titleScene_->Initialize();
	ruleScene_->Initialize();
	operationScene_->Initialize();
	gameScene_->Initialize();
}

void MainSceneManager::Update() {

	switch (currentScene_) {

	case SceneState::Title:
		titleScene_->Update();

		if (titleScene_->IsNextSceneRequested()) {

			if (titleScene_->GetNextScene() == SceneState::Rule) {
				// Title → Rule
				delete titleScene_;
				titleScene_ = nullptr;

				ruleScene_ = new RuleScene();
				ruleScene_->Initialize();
				ruleScene_->Update();

				currentScene_ = SceneState::Rule;
			} else if (titleScene_->GetNextScene() == SceneState::Operation) {
				// Title → Operation
				delete titleScene_;
				titleScene_ = nullptr;

				operationScene_ = new OperationScene();
				operationScene_->Initialize();
				operationScene_->Update();

				currentScene_ = SceneState::Operation;
			} else if (titleScene_->GetNextScene() == SceneState::Game) {
				// Title → Game
				delete titleScene_;
				titleScene_ = nullptr;

				gameScene_ = new GameScene();
				gameScene_->Initialize();
				gameScene_->Update();

				currentScene_ = SceneState::Game;
			}
		}
		break;

	case SceneState::Rule:
		ruleScene_->Update();

		if (ruleScene_->IsNextSceneRequested()) {
			// Rule → Title
			delete ruleScene_;
			ruleScene_ = nullptr;

			titleScene_ = new TitleScene();
			titleScene_->Initialize();
			titleScene_->Update();

			currentScene_ = SceneState::Title;
		}
		break;

	case SceneState::Operation:
		operationScene_->Update();

		if (operationScene_->IsNextSceneRequested()) {
			// Operation → Title
			delete operationScene_;
			operationScene_ = nullptr;

			titleScene_ = new TitleScene();
			titleScene_->Initialize();
			titleScene_->Update();

			currentScene_ = SceneState::Title;
		}
		break;

	case SceneState::Game:
		gameScene_->Update();

		// ★ ここは今は TitleScene に戻すロジックは無いのでこのままでOK
		break;
	}
}




void MainSceneManager::Draw() {
	switch (currentScene_) {
	case SceneState::Title:
		titleScene_->Draw();
		break;
	case SceneState::Rule:
		ruleScene_->Draw();
		break;
	case SceneState::Operation:
		operationScene_->Draw();
		break;
	case SceneState::Game:
		gameScene_->Draw();
		break;
	}
}

void MainSceneManager::Finalize() {
    if (titleScene_) {
        delete titleScene_;
        titleScene_ = nullptr;
    }

    if (ruleScene_) {
        delete ruleScene_;
        ruleScene_ = nullptr;
    }

    if (operationScene_) {
        delete operationScene_;
        operationScene_ = nullptr;
    }

    if (gameScene_) {
        delete gameScene_;
        gameScene_ = nullptr;
    }
}
