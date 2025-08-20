#pragma once
#include "MidEnemy.h"
#include "SmallEnemy.h"
#include "StageBase.h"

class Stage1 : public StageBase {
public:
	void Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelMidEnemy, Player* player) override;
	void Update(KamataEngine::Camera& camera) override;
	void Draw(KamataEngine::Camera& camera) override;
	bool IsStageFinished() const override;

private:
	bool stageFinished_ = false;
	Player* player_ = nullptr;

	int wave_ = 0;
	int timer_ = 0;

	// 敵モデル保存（再出現用）
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelMidEnemy_ = nullptr;

	// --- 追加: 定数（従来の値を定数化するだけで挙動は不変） ---
	static constexpr int kWave0StartFrame = 60;
	static constexpr int kWave0SmallCount = 3;
	static constexpr int kWave1SmallCount = 5;

	// Wave0の出現範囲（従来の -8..+8, -2..+2, 25..30 に一致）
	static constexpr float kW0XMin = -8.0f, kW0XSpan = 16.0f; // -8..+8  → rand()%17
	static constexpr float kW0YMin = -2.0f, kW0YSpan = 4.0f;  // -2..+2  → rand()%5
	static constexpr float kW0ZMin = 25.0f, kW0ZSpan = 5.0f;  // 25..30  → rand()%5

	// Wave1の出現範囲（従来の -10..+10, -3..+3, 27..33 に一致）
	static constexpr float kW1XMin = -10.0f, kW1XSpan = 20.0f; // -10..+10 → rand()%21
	static constexpr float kW1YMin = -3.0f, kW1YSpan = 6.0f;   // -3..+3   → rand()%7
	static constexpr float kW1ZMin = 27.0f, kW1ZSpan = 6.0f;   // 27..33   → rand()%6

private:
	// 敵追加補助
	void AddEnemy(BaseEnemy* enemy);

	// --- 追加: スポーン補助（従来と同じ乱数仕様を内部で使う） ---
	inline float RandLikeIntSteps(float minV, float span, int stepCount) {
		// 従来の: min + (rand() % (stepCount+1)) と等価になるように
		return minV + float(rand() % (stepCount + 1));
	}
	void SpawnSmallEnemies_Shoot(int count);
	void SpawnSmallEnemies_Ram(int count);
};
