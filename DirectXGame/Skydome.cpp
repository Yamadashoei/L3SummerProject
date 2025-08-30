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

	model_ = Model::CreateFromOBJ(objName); // "skydome"
	if (!model_)
		model_ = Model::CreateFromOBJ("skydome/skydome");
	if (!model_) {
		OutputDebugStringA("[Skydome] OBJ load failed\n");
		return;
	}

	wt_.Initialize();
	SetRadius(radius_);
	wt_.translation_ = {0, 0, 0};
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
