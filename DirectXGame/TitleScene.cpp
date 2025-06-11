#include "TitleScene.h"
#include "MainSceneManager.h" // SceneState enum

using namespace KamataEngine;

void TitleScene::Initialize() {
	input_ = Input::GetInstance();

	titleSprite_ = new Sprite();
	titleSprite_ = Sprite::Create(TextureManager::Load("./Resources/Scene/title.png"), {0.0f, 0.0f});

	// ★ Scene切替後は1フレームだけキー入力を無視する
	changeWaitTimer_ = 1;
}

void TitleScene::Update() {
	// ★ 追加：キー入力スキップ用
	if (changeWaitTimer_ > 0) {
		changeWaitTimer_--;
		return;
	}

	if (input_->TriggerKey(DIK_R)) {
		nextSceneRequested_ = true;
		nextScene_ = SceneState::Rule;
	} else if (input_->TriggerKey(DIK_T)) {
		nextSceneRequested_ = true;
		nextScene_ = SceneState::Operation;
	} else if (input_->TriggerKey(DIK_SPACE)) {
		nextSceneRequested_ = true;
		nextScene_ = SceneState::Game;
	}
}

void TitleScene::Draw() {
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	titleSprite_->Draw();
	Sprite::PostDraw();
}
