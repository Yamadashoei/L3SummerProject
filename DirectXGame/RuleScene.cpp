#include "RuleScene.h"

using namespace KamataEngine;

void RuleScene::Initialize() {
	input_ = KamataEngine::Input::GetInstance();

	ruleSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("./Resources/Scene/rule3.png"), {0.0f, 0.0f});

	nextSceneRequested_ = false;
}

void RuleScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		nextSceneRequested_ = true;
	}
}

void RuleScene::Draw() {
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	ruleSprite_->Draw();
	Sprite::PostDraw();
}
