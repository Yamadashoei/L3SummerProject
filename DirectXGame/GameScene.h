#pragma once
#include "KamataEngine.h"

#include "Player.h"
#include "PlayerBullet.h"

#include "BaseEnemy.h"
#include "MidEnemy.h"
#include "SmallEnemy.h"
// #include "BigEnemy.h" // ← BigEnemy作成時に追加予定

#include <list>

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	Camera camera;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelMidEnemy_ = nullptr;
	Model* modelBigEnemy_ = nullptr; // 予備：BigEnemy用（未使用ならnullptrのままでOK）

	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	Player* player_ = nullptr;

	std::list<BaseEnemy*> enemies_; // ★ BaseEnemy*で統一管理
};
