#pragma once
#include "KamataEngine.h"
#include "kMath.h"
#include <string>

class Skydome {
public:
	~Skydome();
	void Initialize(const char* objName, float radius, bool flipInside = true);

	void Update(KamataEngine::Camera& camera);
	void Draw(KamataEngine::Camera& camera);

	// オプション
	void SetRadius(float r);
	void SetSpinY(float yPerFrame) { spinY_ = yPerFrame; }
	void SetFlipInside(bool enable);

	bool IsLoaded() const { return model_ != nullptr; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform wt_;

	float radius_ = 100.0f;
	bool flipInside_ = true; // true なら X を負スケールで反転
	float spinY_ = 0.0f;     // 毎フレ回す量（演出用。0で固定）
};
