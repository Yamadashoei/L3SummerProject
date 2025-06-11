#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"

class Player {

public:
	// デストラクタ
	~Player();

	// 初期化
	void Initialize(KamataEngine::Model* model);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& viewProjection);

private:
	// 発射
	void Attack();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel = nullptr;

	KamataEngine::Input* input_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;
};
