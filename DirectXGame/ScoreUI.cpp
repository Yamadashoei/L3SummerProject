#include "ScoreUI.h"
#include <cassert>
#include <string>

using namespace KamataEngine;

void ScoreUI::Initialize(uint32_t textureHandle, Vector2 pos) {
	const float digitWidth = 32.0f;
	for (int i = 0; i < kNumDigits; ++i) {
		digits_[i] = KamataEngine::Sprite::Create(textureHandle, {0, 0});
		digits_[i]->SetSize({digitWidth, 64});
		digits_[i]->SetPosition({pos.x + i * digitWidth, pos.y});
	}
}

void ScoreUI::Draw(int score) {
	std::string str = std::to_string(score);
	while (str.length() < kNumDigits) {
		str = "0" + str;
	}

	for (int i = 0; i < kNumDigits; ++i) {
		int num = str[i] - '0';
		digits_[i]->SetTextureRect({num * 32.0f, 0.0f}, {32.0f, 64.0f});
		digits_[i]->Draw();
	}
}
