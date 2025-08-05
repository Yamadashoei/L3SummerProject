#pragma once
#include <array>
#include <2d/Sprite.h>

class ScoreUI {
public:
	void Initialize(uint32_t textureHandle, KamataEngine::Vector2 pos);
	void Draw(int score);

private:
	static const int kNumDigits = 5;
	std::array<KamataEngine::Sprite*, kNumDigits> digits_; 
};
