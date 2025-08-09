#pragma once
#include "KamataEngine.h"

#include "Player.h"
#include "PlayerBullet.h"

#include "BaseEnemy.h"
#include "MidEnemy.h"
#include "SmallEnemy.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "StageBase.h"

#include "ScoreManager.h"
#include "ScoreUI.h"

#include <list>

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

enum class GamePhase {
    Playing,
    Result
};

private:
	Camera camera;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelMidEnemy_ = nullptr;
	Model* modelBigEnemy_ = nullptr;

	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	//プレイヤー
	Player* player_ = nullptr;
	//敵
	std::list<BaseEnemy*> enemies_;

	StageBase* stage_ = nullptr;

	// プレイヤー用 HPバー
	Sprite* hpBackSprite_ = nullptr;
	Sprite* hpBarSprite_ = nullptr;

	// レティクル
	Sprite* reticle_ = nullptr;

	// スコア管理
	ScoreManager* scoreManager_ = nullptr;
	ScoreUI* scoreUI_ = nullptr;

	// ゲームフェーズ
	GamePhase phase_ = GamePhase::Playing;

};