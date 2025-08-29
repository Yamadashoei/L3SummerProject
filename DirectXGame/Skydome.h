#pragma once
#include "KamataEngine.h"
#include "kMath.h"
#include <string>

class Skydome {
public:
	~Skydome();

	// objName: Model::CreateFromOBJ に渡す名前（例: "skydome"）
	// radius : 半径（ゲーム空間単位）
	// flipInside: 内側を描くために X を負スケールで反転する保険（BlenderでFlip忘れても見える）
	void Initialize(const char* objName, float radius, bool flipInside = true);

	// カメラの位置に追従＋（任意で）微回転
	void Update(KamataEngine::Camera& camera);

	// 3D描画
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
