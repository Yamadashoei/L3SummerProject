#include "OperationScene.h"

using namespace KamataEngine;

void OperationScene::Initialize() {
	input_ = KamataEngine::Input::GetInstance();

	operationSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("./Resources/Scene/operation2.png"), {0.0f, 0.0f});

	nextSceneRequested_ = false;
}

void OperationScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		nextSceneRequested_ = true;
	}
}

void OperationScene::Draw() {
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	operationSprite_->Draw();
	Sprite::PostDraw();
}
