#pragma once
#include "kMath.h"
#include <2d/Sprite.h>
#include <3d/Camera.h>
#include <base/TextureManager.h>

class EnemyHpBar {
public:
	void Initialize(uint32_t textureHandle);
	void Update(const KamataEngine::Vector3& worldPos, const KamataEngine::Camera& camera, int currentHp, int maxHp);
	void Draw();

	void SetOffset(const KamataEngine::Vector2& offset) { offset_ = offset; }

private:
	KamataEngine::Sprite* back_ = nullptr;
	KamataEngine::Sprite* fill_ = nullptr;

	KamataEngine::Vector2 offset_ = {0.0f, 0.0f};
};
