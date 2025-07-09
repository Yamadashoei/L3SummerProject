#include "EnemyHpBar.h"

using namespace KamataEngine;

void EnemyHpBar::Initialize(uint32_t textureHandle) {
	back_ = Sprite::Create(textureHandle, {0.0f, 0.0f}, {0.2f, 0.2f, 0.2f, 1.0f});
	fill_ = Sprite::Create(textureHandle, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
}

void EnemyHpBar::Update(const Vector3& worldPos, const Camera& camera, int currentHp, int maxHp) {
	Vector2 screenPos = WorldToScreen(worldPos, camera.matView, camera.matProjection, WinApp::kWindowWidth, WinApp::kWindowHeight);

	Vector2 barPos = {screenPos.x + offset_.x, screenPos.y + offset_.y};
	Vector2 backSize = {100.0f, 10.0f};

	float ratio = static_cast<float>(currentHp) / maxHp;
	if (ratio < 0.0f)
		ratio = 0.0f;
	if (ratio > 1.0f)
		ratio = 1.0f;

	Vector2 fillSize = {100.0f * ratio, 10.0f};

	if (back_) {
		back_->SetPosition(barPos);
		back_->SetSize(backSize);
	}
	if (fill_) {
		fill_->SetPosition(barPos);
		fill_->SetSize(fillSize);
	}
}

void EnemyHpBar::Draw() {
	if (back_)
		back_->Draw();
	if (fill_)
		fill_->Draw();
}
