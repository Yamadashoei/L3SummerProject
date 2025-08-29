#include "Skydome.h"
#include <Windows.h> // OutputDebugStringA
#include <base/TextureManager.h>

using namespace KamataEngine;

Skydome::~Skydome() {
	delete model_;
	model_ = nullptr;
}

void Skydome::Initialize(const char* objName, float radius, bool flipInside) {
	radius_ = radius;
	flipInside_ = flipInside;

	// 読み込み（パス違いの保険：2パターン試す）
	model_ = Model::CreateFromOBJ(objName);
	if (!model_) {
		// 例: "skydome/skydome" 直指定が必要な実装向け
		model_ = Model::CreateFromOBJ("skydome");
	}
	if (!model_) {
		OutputDebugStringA("[Skydome] OBJ load failed.\n");
		return;
	}

	wt_.Initialize();
	SetRadius(radius_);

	// 最初は原点（Updateでカメラに追従）
	wt_.translation_ = {0.0f, 0.0f, 0.0f};
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Skydome::SetRadius(float r) {
	radius_ = r;
	// 内側が見えるように（Flip忘れ対策で X を反転可能）
	wt_.scale_ = flipInside_ ? Vector3{-radius_, radius_, radius_} : Vector3{radius_, radius_, radius_};
}

void Skydome::SetFlipInside(bool enable) {
	flipInside_ = enable;
	SetRadius(radius_);
}

void Skydome::Update(Camera& camera) {
	// カメラ中心に追従＋微回転（任意）
	wt_.translation_ = camera.translation_;
	wt_.rotation_.y += spinY_;
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Skydome::Draw(Camera& camera) {
	if (!model_) {
		return;
	}
	model_->Draw(wt_, camera);
}
